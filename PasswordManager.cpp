//============================================================================
// Name        : 2223S_19c_s26941.cpp
// Author      : Taha Burak Sahin
//============================================================================



#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <random>

using namespace std;


string encrypt(const string& message) {
    string encryptedMessage = message;

    for (char& c : encryptedMessage) {
        if (isalpha(c)) {
            if (islower(c)) {
                c = (c + 1 - 'a') % 26 + 'a';
            } else {
                c = (c + 1 - 'A') % 26 + 'A';
            }
        } else if (isdigit(c)) {
            c = (c + 1 - '0') % 10 + '0';
        }
    }

    return encryptedMessage;
}

string decrypt(const string& encryptedMessage) {
    string decryptedMessage = encryptedMessage;

    for (char& c : decryptedMessage) {
        if (isalpha(c)) {
            if (islower(c)) {
                c = (c - 1 - 'a' + 26) % 26 + 'a';
            } else {
                c = (c - 1 - 'A' + 26) % 26 + 'A';
            }
        } else if (isdigit(c)) {
            c = (c - 1 - '0' + 10) % 10 + '0';
        }
    }

    return decryptedMessage;
}


struct PasswordEntry {
    string website;
    string loginEmail;
    string password;
    time_t creationDate;
    time_t modificationDate;
};

bool validatePassword(const string& password) {

    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;


    for (char c : password) {
        if (isupper(c))
            hasUpperCase = true;
        else if (islower(c))
            hasLowerCase = true;
        else if (isdigit(c))
            hasDigit = true;
        else if (ispunct(c))
            hasSpecialChar = true;
    }


    bool isValid = hasUpperCase && hasLowerCase && hasDigit && hasSpecialChar && password.length() >= 6;
    return isValid;
}

void addNewPassword(vector<PasswordEntry>& passwords, const string& userEmail) {
    PasswordEntry entry;
    cout << "Website Address: ";
    cin.ignore();
    getline(cin, entry.website);
    entry.loginEmail = userEmail;

    cout << "Email: ";
    getline(cin, entry.loginEmail);

    cout << "Password: ";
    getline(cin, entry.password);

    if (!validatePassword(entry.password)) {
        cout << "Invalid password! Password must contain at least one uppercase letter, one lowercase letter, one digit, one special character, and be at least 6 characters long." << endl;
        return;
    }

    bool isSamePassword = false;
    for (const PasswordEntry& passwordEntry : passwords) {
        if (passwordEntry.loginEmail == userEmail && passwordEntry.password == entry.password && passwordEntry.website == entry.website) {
            isSamePassword = true;
            break;
        }
    }

    if (isSamePassword) {
        cout << "Same password already exists for this website. Please enter a different password." << endl;
        return;
    }

    for (const PasswordEntry& passwordEntry : passwords) {
        if (passwordEntry.loginEmail == userEmail && passwordEntry.password == entry.password) {
            cout << "Same password already exists for another website. Please enter a different password." << endl;
            return;
        }
    }

    time_t now = time(0);
    entry.creationDate = now;
    entry.modificationDate = now;

    passwords.push_back(entry);
    cout << "Password added!" << endl;

    ofstream file(userEmail + ".txt");
    if (file.is_open()) {
        for (const PasswordEntry& entry : passwords) {
            if (entry.loginEmail == userEmail) {
                file << "Website: " << entry.website << endl;
                file << "Login Email: " << entry.loginEmail << endl;
                file << "Password: " << entry.password << endl;
                file << "Creation Date: " << put_time(localtime(&entry.creationDate), "%c") << endl;
                file << "Modification Date: " << put_time(localtime(&entry.modificationDate), "%c") << endl;
                file << "-------------------" << endl;
            }
        }
        file.close();
    } else {
        cout << "Unable to open the file!" << endl;
    }
}

void showPasswords(const vector<PasswordEntry>& passwords, const string& userEmail) {
    if (passwords.empty()) {
        cout << "No passwords saved!" << endl;
        return;
    }

    string searchKeyword;
    vector<PasswordEntry> matchedPasswords;

    cin.ignore();

    cout << "Enter a keyword to search for passwords: ";
    getline(cin, searchKeyword);

    for (const PasswordEntry& entry : passwords) {
        if (entry.loginEmail == userEmail && (entry.website.find(searchKeyword) != string::npos || entry.password.find(searchKeyword) != string::npos)) {
            matchedPasswords.push_back(entry);
        }
    }

    if (matchedPasswords.empty()) {
        cout << "No matching passwords found!" << endl;
        return;
    }

    cout << "Matching Passwords:" << endl;
    for (const PasswordEntry& entry : matchedPasswords) {
        cout << "Website: " << entry.website << endl;
        cout << "Login Email: " << entry.loginEmail << endl;
        cout << "Password: " << entry.password << endl;
        cout << "Creation Date: " << put_time(localtime(&entry.creationDate), "%c") << endl;
        cout << "Modification Date: " << put_time(localtime(&entry.modificationDate), "%c") << endl;
        cout << "-------------------" << endl;
    }
}

void modifyPassword(vector<PasswordEntry>& passwords, const string& userEmail) {
    cout << "Enter the website address to modify the password: ";
    string website;
    cin.ignore();
    getline(cin, website);

    bool passwordModified = false;

    for (PasswordEntry& entry : passwords) {
        if (entry.loginEmail == userEmail && entry.website == website) {
            cout << "Current Password: " << entry.password << endl;
            cout << "Enter the new password: ";
            getline(cin, entry.password);
            entry.modificationDate = time(0);
            passwordModified = true;
            break;
        }
    }

    if (passwordModified) {
        ofstream file(userEmail + ".txt");
        if (file.is_open()) {
            for (const PasswordEntry& entry : passwords) {
                if (entry.loginEmail == userEmail) {
                    file << "Website: " << entry.website << endl;
                    file << "Login Email: " << entry.loginEmail << endl;
                    file << "Password: " << entry.password << endl;
                    file << "Creation Date: " << put_time(localtime(&entry.creationDate), "%c") << endl;
                    file << "Modification Date: " << put_time(localtime(&entry.modificationDate), "%c") << endl;
                    file << "-------------------" << endl;
                }
            }
            file.close();
        } else {
            cout << "Unable to open the file!" << endl;
        }

        cout << "Password modified successfully!" << endl;
    } else {
        cout << "No matching password found for the given website address!" << endl;
    }
}

void deleteAccount(vector<PasswordEntry>& passwords, const string& userEmail) {
    auto entry = passwords.begin();
    while (entry != passwords.end()) {
        if (entry->loginEmail == userEmail) {
            entry = passwords.erase(entry);
        } else {
            ++entry;
        }
    }

    if (remove((userEmail + ".txt").c_str()) != 0) {
        cout << "Unable to delete the account file!" << endl;
    } else {
        cout << "Account deleted successfully!" << endl;
    }
}

bool verifyTwoFactorCode() {
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(100000, 999999);

    int verificationCode = distribution(generator);
    cout << "Verification Code: " << verificationCode << endl;

    int code;
    cout << "Enter the verification code: ";
    cin >> code;

    if (code == verificationCode) {
        cout << "Verification successful!" << endl;
        return true;
    } else {
        cout << "Invalid verification code!" << endl;
        return false;
    }
}

class RegisterPart {
public:
    void registerAccount() {
        string email, password;
        cout << "Email: ";
        cin >> email;
        cout << "Your password must contain at least one uppercase letter, one lowercase letter, \n one number, one special character and must be at least 6 characters long." << endl;
        cout << "Password: ";
        cin >> password;

        if (!validatePassword(password)) {
            cout << "Invalid password! Password must contain at least one uppercase letter, one lowercase letter, one digit, one special character, and be at least 6 characters long." << endl;
            return;
        }
        string searchmailfile = encrypt(email);
        ifstream file(searchmailfile + ".txt");
        if (file) {
            cout << "An account already exists with this email!" << endl;
            file.close();
            return;
        }

        ofstream outFile(searchmailfile + ".txt", ios::app);
        if (outFile.is_open()) {
            time_t now = time(0);

            char* dateTime = ctime(&now);
            string newmail = encrypt(email);
            string newpassword = encrypt(password);

            outFile << "Email: " << newmail << endl;
            outFile << "Password: " << newpassword << endl;
            outFile << "Registration Date: " << dateTime;
            outFile << "-------------------" << endl;

            file.close();
            cout << "Account created!" << endl;
        } else {
            cout << "Unable to open the file!" << endl;
        }
    }
};

class LoginPart {
public:
    bool loginAccount(string& userEmail) {
        string email, password, searchtxtemail;
        cout << "Email: ";
        cin >> email;
        cout << "Password: ";
        cin >> password;
        searchtxtemail = encrypt(email);
        ifstream file(searchtxtemail + ".txt");
        string line;
        string storedEmail, storedPassword;

        while (getline(file, line)) {

            if (line.find("Email: ") != string::npos) {
                storedEmail = line.substr(7);
            } else if (line.find("Password: ") != string::npos) {
                storedPassword = line.substr(10);
            }
            else if (line.find("-------------------") != string::npos) {
                string newemail = decrypt(storedEmail);
                string newPassword = decrypt(storedPassword);
                if (email == newemail && password == newPassword) {
                    file.close();
                    cout << "Login successful!" << endl;

                    if (verifyTwoFactorCode()) {
                        userEmail = email;
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
        file.close();
        return false;
    }
};

int main() {
    int choice;
    bool loggedIn = false;
    string userEmail;

    vector<PasswordEntry> passwords;

    RegisterPart registerPart;
    LoginPart loginPart;

    while (true) {
        if (!loggedIn) {
            cout << "Login Panel" << endl;
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    registerPart.registerAccount();
                    break;
                case 2:
                    if (loginPart.loginAccount(userEmail)) {
                        loggedIn = true;
                        cout << "Login successful!" << endl;
                    } else {
                        cout << "Incorrect email or password!" << endl;
                    }
                    break;
                case 3:
                    cout << "Exiting the program..." << endl;
                    return 0;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } else {
            cout << "Password Manager" << endl;
            cout << "1. Add New Password" << endl;
            cout << "2. Show Passwords" << endl;
            cout << "3. Modify Password" << endl;
            cout << "4. Delete Account" << endl;
            cout << "5. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addNewPassword(passwords, userEmail);
                    break;
                case 2:
                    showPasswords(passwords, userEmail);
                    break;
                case 3:
                    modifyPassword(passwords, userEmail);
                    break;
                case 4:
                    deleteAccount(passwords, userEmail);
                    loggedIn = false;
                    break;
                case 5:
                    loggedIn = false;
                    cout << "Logged out!" << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        }
    }
}
