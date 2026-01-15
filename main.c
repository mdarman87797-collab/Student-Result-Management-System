#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
    char name[50];
    int roll;
    char dept[20];
    int sem;
    char user[20];
    char pass[20];
    char role[10];
    float marks[5];
    float gpa;
};

void regStudent();
int login(char type[]);
void resetPass();
void giveMarks();
void showResult();
void adminMenu();
char *grade(float gpa);
void stop();
void clr();
void createDefaultAdmin();

int main()
{
    createDefaultAdmin();

    int op;
    while (1)
    {
        clr();
        printf("\n==============================\n");
        printf("   STUDENT RESULT SYSTEM\n");
        printf("==============================\n");
        printf("1. Register\n");
        printf("2. Student Login\n");
        printf("3. Admin Login\n");
        printf("4. Reset Password\n");
        printf("5. Exit\n");
        printf("------------------------------\n");
        printf("Enter option: ");
        if (scanf("%d", &op) != 1)
        {

            while (getchar() != '\n')
                ;
            printf("\nInvalid input!\n");
            stop();
            continue;
        }

        switch (op)
        {
        case 1:
            regStudent();
            break;
        case 2:
            if (login("student"))
                showResult();
            else
                printf("\nWrong student login!\n");
            stop();
            break;
        case 3:
            if (login("admin"))
                adminMenu();
            else
                printf("\nWrong admin login!\n");
            stop();
            break;
        case 4:
            resetPass();
            break;
        case 5:
            exit(0);
        default:
            printf("\nInvalid option!\n");
            stop();
        }
    }
    return 0;
}

void createDefaultAdmin()
{
    FILE *fp = fopen("students.dat", "ab+");
    if (!fp)
    {
        printf("Unable to open data file!\n");
        return;
    }

    struct Student s;
    int found = 0;

    fseek(fp, 0, SEEK_SET);
    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        if (strcmp(s.role, "admin") == 0)
        {
            found = 1;
            break;
        }
    }

    if (!found)
    {

        memset(&s, 0, sizeof(s));
        strcpy(s.name, "Admin");
        s.roll = 0;
        strcpy(s.dept, "admin");
        s.sem = 0;
        strcpy(s.role, "admin");
        s.gpa = 0.0f;


        printf("\n=== SETUP ADMIN ACCOUNT ===\n");
        printf("Enter admin username: ");
        scanf("%19s", s.user);
        printf("Enter admin password: ");
        scanf("%19s", s.pass);


        fseek(fp, 0, SEEK_END);
        if (fwrite(&s, sizeof(s), 1, fp) == 1)
        {
            printf("\nAdmin account created successfully!\n");
            printf("You can change admin password later from Reset Password menu.\n");
        }
        else
        {
            printf("Failed to create admin account!\n");
        }
        getchar();
        printf("Press Enter to continue...");
        getchar();
    }

    fclose(fp);
}

void regStudent()
{
    FILE *fp = fopen("students.dat", "ab");
    if (!fp)
    {
        printf("File error!\n");
        return;
    }

    struct Student s;
    clr();
    printf("\n=== Register Student ===\n");
    getchar();
    printf("Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Roll: ");
    scanf("%d", &s.roll);
    printf("Dept: ");
    scanf("%s", s.dept);
    printf("Semester: ");
    scanf("%d", &s.sem);
    printf("Username: ");
    scanf("%19s", s.user);
    printf("Password: ");
    scanf("%19s", s.pass);
    strcpy(s.role, "student");
    s.gpa = 0.0;

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
    printf("\nStudent Registered!\n");
    stop();
}

int login(char type[])
{
    FILE *fp = fopen("students.dat", "rb");
    if (!fp)
    {
        printf("No data!\n");
        return 0;
    }

    struct Student s;
    char u[20], p[20];
    clr();

    printf("\n=== %s LOGIN ===\n", strcmp(type, "admin") == 0 ? "ADMIN" : "STUDENT");
    printf("Username: ");
    scanf("%19s", u);
    printf("Password: ");
    scanf("%19s", p);

    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        if (strcmp(s.user, u) == 0 && strcmp(s.pass, p) == 0 && strcmp(s.role, type) == 0)
        {
            fclose(fp);
            printf("\nWelcome %s!\n", s.name);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void resetPass()
{
    FILE *fp = fopen("students.dat", "rb+");
    if (!fp)
    {
        printf("No file found!\n");
        return;
    }

    struct Student s;
    char u[20], np[20];
    int found = 0;
    clr();
    printf("\n=== RESET PASSWORD ===\n");
    printf("Username: ");
    scanf("%19s", u);

    /* search file */
    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        if (strcmp(s.user, u) == 0)
        {
            printf("New password: ");
            scanf("%19s", np);
            strcpy(s.pass, np);
            fseek(fp, -((long)sizeof(s)), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            fflush(fp);
            printf("\nPassword changed!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nUser not found!\n");
    fclose(fp);
    stop();
}

char *grade(float gpa)
{
    if (gpa >= 3.75)
        return "A+";
    else if (gpa >= 3.50)
        return "A";
    else if (gpa >= 3.25)
        return "A-";
    else if (gpa >= 3.00)
        return "B+";
    else if (gpa >= 2.75)
        return "B";
    else if (gpa >= 2.50)
        return "C";
    else
        return "F";
}

void giveMarks()
{
    FILE *fp = fopen("students.dat", "rb+");
    if (!fp)
    {
        printf("No student data!\n");
        return;
    }

    struct Student s;
    int roll, found = 0;
    clr();
    printf("\n=== ENTER MARKS ===\n");
    printf("Roll: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        if (s.roll == roll && strcmp(s.role, "student") == 0)
        {
            float total = 0;
            for (int i = 0; i < 5; i++)
            {
                printf("Subject %d mark: ", i + 1);
                scanf("%f", &s.marks[i]);
                total += s.marks[i];
            }
            s.gpa = (total / 500.0f) * 4.0f;
            fseek(fp, -((long)sizeof(s)), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            fflush(fp);
            printf("\nMarks updated! GPA: %.2f (%s)\n", s.gpa, grade(s.gpa));
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nRoll not found!\n");
    fclose(fp);
    stop();
}

void showResult()
{
    FILE *fp = fopen("students.dat", "rb");
    if (!fp)
    {
        printf("No data!\n");
        return;
    }

    struct Student s;
    clr();
    printf("\n=== STUDENT RESULTS ===\n");
    printf("-------------------------------------------\n");
    printf("Roll\tName\t\tGPA\tGrade\n");
    printf("-------------------------------------------\n");

    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        if (strcmp(s.role, "student") == 0)
            printf("%d\t%-12s\t%.2f\t%s\n", s.roll, s.name, s.gpa, grade(s.gpa));
    }

    printf("-------------------------------------------\n");
    fclose(fp);
    stop();
}

void adminMenu()
{
    int op;
    while (1)
    {
        clr();
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Register Student\n");
        printf("2. Enter Marks\n");
        printf("3. View Results\n");
        printf("4. Reset Password\n");
        printf("5. Logout\n");
        printf("-------------------\n");
        printf("Option: ");
        if (scanf("%d", &op) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input!\n");
            stop();
            continue;
        }

        switch (op)
        {
        case 1:
            regStudent();
            break;
        case 2:
            giveMarks();
            break;
        case 3:
            showResult();
            break;
        case 4:
            resetPass();
            break;
        case 5:
            return;
        default:
            printf("Invalid!\n");
            stop();
        }
    }
}

void stop()
{
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void clr()
{
#ifdef _WIN32

#else
    system("clear");
#endif
}
