#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <filesystem>

using namespace std;

///file or directory
bool is_directory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

string to_lower(string str)
{
    string lower_case_string;
    for(char c : str)
    {
        lower_case_string += tolower(c);
    }
    return lower_case_string;
}

///create and return new directory to store new files
string get_create_new_dir( char* dir_name, string new_extension)
{

    string new_dir = "\\files_with_" + new_extension + "_extension";

    int n = strlen(dir_name ) + (new_dir.length());

    char char_array[n + 1];
    strcpy(char_array, dir_name);
    strcat(char_array, new_dir.c_str());
    mkdir(char_array);
    return char_array;
}

///create and store new file with new extension
void create_renamed_ext_file(char  *dname,string target_path,string dest_folder,  string new_extension)
{
    char* token_name = strtok(dname,"." );
    string new_file_path =dest_folder + '\\' + token_name+ '.' + new_extension;

    ifstream f1(target_path,ios::binary);
    ofstream f2(new_file_path,ios::binary);

    f2<<f1.rdbuf();

    f1.close();
    f2.close();
}

void explore(char* dir_name, string target_extension, string new_extension)
{
    DIR *dir;
    struct dirent *entry;

    ///open directory
    dir = opendir(dir_name);
    if(!dir)
    {
        cout<<"Directory not found!"<<endl;
        return;
    }
    ///folder used to save renamed files
    string new_dest_folder;
    bool dest_folder_created = false;
    ///iterate on files in the directory
    while((entry = readdir(dir)) != NULL)
    {
        if(entry->d_name[0] != '.')
        {
            string target_file_path = (string)dir_name + '\\' + entry->d_name;

            ///Recursive call if it is a directory
            if(is_directory((char*)target_file_path.c_str()))
            {
                explore((char*)target_file_path.c_str(), target_extension, new_extension);
            }
            ///is a file
            else
            {
                ///operate on the target extension only
                string extenstion_of_file = target_file_path.substr(target_file_path.find_last_of(".") + 1);
                if(to_lower(extenstion_of_file) ==  to_lower(target_extension))
                {
                    if(!dest_folder_created)
                    {
                        new_dest_folder = get_create_new_dir(dir_name, new_extension);
                        dest_folder_created = true;
                    }
                    create_renamed_ext_file(entry->d_name, target_file_path,new_dest_folder, new_extension);
                }
            }
        }
    }
    closedir(dir);
}

int main()
{
    string target_path, target_extension, new_extension;
    cout<<"Enter the folder path: ";
    cin>> target_path;

    cout<<"Enter the target extension to be replaced: ";
    cin>> target_extension;

    cout<<"Enter the new extension: ";
    cin>> new_extension;

    explore((char*)target_path.c_str(),  target_extension, new_extension);

    return 0;
}
