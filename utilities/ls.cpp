
//linux ls program
//currently have long option, inode option and color for files and executables, and all files
//g++ ls.cpp -std=c++17 -lstdc++fs

#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>
#define RESET "\033[0m"
#define BLUE "\033[34m" /* Blue */
#define GREEN "\033[32m"
#define PRINT_COLOR(x, color) std::cout << color << x << RESET;

std::string permissions_string(std::filesystem::perms p, bool is_dir = false)
{

    std::string result = (!is_dir) ? "-" : "d";
    result += ((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-");
    result += ((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-");
    result += ((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-");
    result += ((p & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-");
    result += ((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-");
    result += ((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-");
    result += ((p & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-");
    result += ((p & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-");
    result += ((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-");
    return result;
}
void usage()
{
    std::cout << "this is ls program, the program will list a directory\n" <<
        " (l) is for long option\n (i) for inode\n (a) for all\n (h) for human readable\n" <<
        "for example: ls -lh\n";
}

int main(int argc, char *argv[])
{
    std::stringstream buffer;
    bool long_mod = false;
    bool human_read = false;
    bool inode = false;
    bool all_mod = false;
    char t[ 100 ] = "";

    if (argc > 1)
    {
        std::string option = std::string(argv[1]);
        for(auto letter : option) {
            switch(letter)
            {
                case 'l': long_mod = true;break;
                case 'h': human_read = true;break;
                case 'a': all_mod = true;break;
                case 'i': inode = true;break;
                case '-':break;
                default:
                    std::cout << "error, there's no such option please check usage\n";
                    usage();
                    exit(0);
            }
        }
    }
    for (auto &p : std::filesystem::directory_iterator("."))
    {
        buffer << p;
        std::filesystem::file_status perm = status(p);
        std::string file_name = buffer.str().substr(3);
        file_name.pop_back();
        struct stat b;
        stat(file_name.c_str(), &b);
        if(all_mod && file_name[0] == '.'){continue;}
        if (long_mod)
        {
            if(inode){std::cout << b.st_ino << " ";}
            std::cout << permissions_string(perm.permissions(), p.is_directory()) << " ";
            if (!p.is_directory())
            {
                if(human_read)
                    std::cout << std::setprecision(2) << std::filesystem::file_size(p)/1000.0 << "K ";
                else
                    std::cout << std::filesystem::file_size(p) << " ";
            }

            strftime(t, 100, "%b %d %H:%M", localtime(&b.st_mtime));
            std::cout << t << " ";

        }
        PRINT_COLOR(file_name + "\n", (p.is_directory() ? BLUE : (b.st_mode & 0111) ? GREEN : RESET));
        buffer.str("");
    }
    //std::cout << get_current_dir_name() << std::endl;
}