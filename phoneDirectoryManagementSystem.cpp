#include <iostream>
#include <string>
#include <filesystem>
#include <limits>
#include <fstream>  // For file operations

namespace fs = std::filesystem;
using namespace std;

class Directory
{
public: 
    long phoneno;
    string fname,  lname;

    Directory(long phoneno = 0,  string fname = "",  string lname = "")
        :  phoneno(phoneno),  fname(fname),  lname(lname) {}
};

void listAllFiles(const string& path) {
    cout << "List of all Files: \n";
    for (const auto& entry : fs::directory_iterator(path)) {
        cout << entry.path().filename().string() << endl;
    }
}

void listFilesByExtension(const string& path,  const string& extension) {
    cout << "Listing Files with Extension:  " << extension << endl;
    int count = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (fs::is_regular_file(entry) && entry.path().extension() == extension) {
            cout << entry.path().filename().string() << endl;
            count++;
        }
    }
    if (count == 0)
        cout << "No files found with extension " << extension << endl;
}

void listFilesByPattern(const string& path,  const string& pattern) {
    cout << "Listing Files Matching Pattern:  " << pattern << endl;
    int count = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (fs::is_regular_file(entry) && entry.path().filename().string().find(pattern) != string::npos) {
            cout << entry.path().filename().string() << endl;
            count++;
        }
    }
    if (count == 0)
        cout << "No files found matching pattern " << pattern << endl;
}

void createDirectory(const string& path) {
    if (fs::create_directories(path)) { // Use create_directories for nested paths
        cout << "Directory Successfully Created:  " << path << endl;
    } else {
        cout << "Failed to create directory or directory already exists." << endl;
    }
}

void createFile(const string& path) {
    ofstream file(path);
    if (file) {
        cout << "File successfully created:  " << path << endl;
    } else {
        cout << "Failed to create file:  " << path << endl;
    }
}

void changeDirectory(string& path) {
    int choice;
    cout << "Change Directory Options: \n";
    cout << "_____________\n";
    cout << "1. Step by step backward.\n";
    cout << "2. Go to Root Directory.\n";
    cout << "3. Forward directory.\n";
    cout << "Enter your choice:  ";
    while (!(cin >> choice) || (choice < 1 || choice > 3)) {
        cout << "Invalid input. Please enter a number between 1 and 3:  ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    switch (choice) {
        case 1: 
            path = fs::current_path().parent_path().string();
            break;
        case 2: 
            path = fs::path("/").string();
            break;
        case 3:  {
            string newPath;
            cout << "Enter the path of the directory:  ";
            cin.ignore(); // Ignore the newline character from previous input
            getline(cin, newPath);
            if (fs::exists(newPath) && fs::is_directory(newPath)) {
                path = newPath;
            } else {
                cout << "Invalid path or directory does not exist." << endl;
                return;
            }
            break;
        }
    }
    fs::current_path(path);
    cout << "Current directory changed to:  " << fs::current_path() << endl;
}

int main() {
    string currentPath = fs::current_path().string();
    int ch;

    cout << "\nWelcome to Directory and File Management System\n";
    do {
        cout << "\nMain Menu\n";
        cout << "_____________\n";
        cout << "1. Display List of Files\n";
        cout << "2. Create a New Directory\n";
        cout << "3. Create a New File\n";
        cout << "4. Change the Working Directory\n";
        cout << "5. Exit\n";
        cout << "Enter Your Choice:  ";
        while (!(cin >> ch) || (ch < 1 || ch > 5)) {
            cout << "Invalid input. Please enter a number between 1 and 5:  ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (ch) {
            case 1:  {
                int subChoice;
                cout << "\nList Files Detail\n";
                cout << "_______________\n";
                cout << "1. List all files\n";
                cout << "2. List files by extension\n";
                cout << "3. List files by name pattern\n";
                cout << "Enter your choice:  ";
                while (!(cin >> subChoice) || (subChoice < 1 || subChoice > 3)) {
                    cout << "Invalid input. Please enter a number between 1 and 3:  ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                switch (subChoice) {
                    case 1: 
                        listAllFiles(currentPath);
                        break;
                    case 2:  {
                        string extension;
                        cout << "Enter the file extension (e.g.,  .txt):  ";
                        cin >> extension;
                        if (extension.front() != '.') {
                            extension = '.' + extension;
                        }
                        listFilesByExtension(currentPath, extension);
                        break;
                    }
                    case 3:  {
                        string pattern;
                        cout << "Enter the file name pattern:  ";
                        cin >> pattern;
                        listFilesByPattern(currentPath, pattern);
                        break;
                    }
                }
                break;
            }
            case 2:  {
                string dirName;
                cout << "Enter the name of the directory:  ";
                cin >> dirName;
                createDirectory(currentPath + "/" + dirName);
                break;
            }
            case 3:  {
                string fileName;
                cout << "Enter the name of the file to create:  ";
                cin >> fileName;
                createFile(currentPath + "/" + fileName);
                break;
            }
            case 4: 
                changeDirectory(currentPath);
                break;
            case 5: 
                cout << "Thank you for using the system. Exiting...\n";
                break;
            default: 
                cout << "Invalid option! Please try again.\n";
        }
    } while (ch != 5);

    return 0;
}

