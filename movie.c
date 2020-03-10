#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_SIZE 500
#define DEF_FNAME1 "movies.dat"
#define DEF_FNAME2 "tags.dat"

int Numbers_of_Tag = 0;
int Numbers_of_Movie = 0;

struct _MovieGenre
{
	char Genre[100];
};
typedef struct _MovieGenre MovieGenre;

struct _MovieDatLink {
	int MovieID;
	int CountTag;
	char* Title;
	char* Genre;
	MovieGenre MovieGenre[10];
};
typedef struct _MovieDatLink MovieDatLink;

struct _TagDatLink {
	int UserID;
	int MovieID;
	char* Tags;
	char* TimeStamp;
};
typedef struct _TagDatLink TagDatLink;

MovieDatLink MovieDat[11000];
TagDatLink TagDat[100000];
MovieDatLink MovieSortDat[11000];

void Run();
int SelectMenu();
int getnum();
void MovieToken();
void TagToken();
void GenreToken();
void FindMovie1();
void FindMovie2();
void FindTag();
void CountTag();
void PopularMovie();

int main() {
	clock_t start, end;
	double result;
	start = clock();
	MovieToken();
	TagToken();
	GenreToken();
	CountTag();
	end = clock();
	result = (double)(end - start);
	//printf("Running Time: %f\n", result);
	Run();
	return 0;
}

void MovieToken()
{
	int StrLength = 0;
	FILE* fp = fopen(DEF_FNAME1, "r");
	char buf[BUFFER_SIZE + 1];
	char* token;
	for (int i = 0; i < 30000; i++)
	{
		Numbers_of_Movie++;
		if (fgets(buf, BUFFER_SIZE + 100, fp) == NULL)
		{
			//printf("error or end of file\n");
			break;
		}
		token = strtok(buf, ";;");
		MovieDat[i].MovieID = atoi(token);
		token = strtok(NULL, ";;");
		StrLength = strlen(token);
		MovieDat[i].Title = (char*)malloc(sizeof(char) * StrLength);
		strcpy(MovieDat[i].Title, token);
		token = strtok(NULL, ";;");
		StrLength = strlen(token);
		MovieDat[i].Genre = (char*)malloc(sizeof(char) * StrLength);
		strcpy(MovieDat[i].Genre, token);
		//printf("%d\n", MovieDat[i].MovieID);
	}
	//printf("%d\n", Numbers_of_Movie);
	fclose(fp);
}

void TagToken()
{
	int StrLength = 0;
	FILE* fp = fopen(DEF_FNAME2, "r");
	char* token;
	char buf[501];
	for (int i = 0; i < 100000; i++) {
		Numbers_of_Tag++;
		if (fgets(buf, 500, fp) == NULL) {
			//printf("error or end of file\n");
			break;
		}
		token = strtok(buf, "::");
		TagDat[i].UserID = atoi(token);
		token = strtok(NULL, "::");
		TagDat[i].MovieID = atoi(token);
		token = strtok(NULL, "::");
		StrLength = strlen(token);
		TagDat[i].Tags = (char*)malloc(sizeof(char) * StrLength);
		strcpy(TagDat[i].Tags, token);
		token = strtok(NULL, "::");
		StrLength = strlen(token);
		TagDat[i].TimeStamp = (char*)malloc(sizeof(char) * StrLength);
		strcpy(TagDat[i].TimeStamp, token);
		//printf("%d %d %s %s", TagDat[i].UserID, TagDat[i].MovieID, TagDat[i].Tags, TagDat[i].TimeStamp);
	}
	//printf("%d\n", Numbers_of_Tag);
	fclose(fp);
}

void GenreToken()
{
	int StrLength = 0;
	char* token;
	char buf[BUFFER_SIZE];
	for (int i = 0; i < Numbers_of_Movie - 1; i++) {
		int j = 0;
		token = strtok(MovieDat[i].Genre, "|");
		strcpy(MovieDat[i].MovieGenre[j].Genre, token);
		for (j = 1; j < 10; j++) {
			token = strtok(NULL, "|");
			if (token == NULL)
			{
				break;
			}
			strcpy(MovieDat[i].MovieGenre[j].Genre, token);
		}
	}
}

void FindMovie1()
{
	int i, j, idx, count = 0;
	char tagdata[30];
	printf("Enter tag: ");
	scanf(" %[^\n]s", tagdata);
	for (i = 0; i < Numbers_of_Tag - 1; i++)
	{
		if (strcmp(TagDat[i].Tags, tagdata) == 0)
		{
			count++;
			idx = TagDat[i].MovieID;
			for (j = 0; j < Numbers_of_Movie - 1; j++)
			{
				if (MovieDat[j].MovieID == idx)
					printf("Movie Title: %s\n", MovieDat[j].Title);
			}
		}
	}
	if (count == 0)
	{
		printf("There is no movie for the tag.\n");
	}
}

void FindMovie2()
{
	int i, j, count = 0;
	char genredata[15];
	printf("----------------------- < Genre List > -----------------------\n");
	printf("Action / Adventure / Animation / Children / Comedy / Crime\n");
	printf("Documentary / Drama / Fantasy / Film-Noir / Horror / Musical\n");
	printf("Mystery / Romance / Sci-Fi / Thriller / War\n\n");
	printf("Enter genre: ");
	scanf(" %[^\n]s", genredata);
	for (i = 0; i < Numbers_of_Movie - 1; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (strcmp(MovieDat[i].MovieGenre[j].Genre, genredata) == 0)
			{
				count++;
				printf("Movie Title: %s\n", MovieDat[i].Title);
			}
		}
	}
	if (count == 0)
	{
		printf("This genre does not exist.\n");
	}
}

void FindTag()
{
	int i, j, idx, count = 0;
	char moviedata[60];
	printf("Enter movie title (year): ");
	scanf(" %[^\n]s", moviedata);
	for (i = 0; i < Numbers_of_Movie - 1; i++)//Number of Movie에서 -1하기 기억하기.
	{
		if (strcmp(MovieDat[i].Title, moviedata) == 0)
		{
			count++;
			idx = MovieDat[i].MovieID;
			for (j = 0; j < Numbers_of_Tag - 1; j++)
			{
				if (TagDat[j].MovieID == idx)
					printf("Tag: %s\n", TagDat[j].Tags);
			}
		}
	}
	if (count == 0)
	{
		printf("There is no tag for the movie.\n");
	}
}

void CountTag()
{
	int i, j, idx, count = 0;
	for (i = 0; i < Numbers_of_Movie - 1; i++)//Number of Movie에서 -1하기 기억하기.
	{
		count = 0;
		idx = MovieDat[i].MovieID;
		for (j = 0; j < Numbers_of_Tag; j++)
		{
			if (TagDat[j].MovieID == idx)
				count++;
		}
		MovieDat[i].CountTag = count;
		//printf("%3d", MovieDat[i].CountTag);
	}
}

void PopularMovie()
{
	int loop, i, j, k, count = 0;
	int popmovie[10000];
	MovieDatLink hold;
	for (int i = 0; i < Numbers_of_Movie - 1; i++)
	{
		MovieSortDat[i] = MovieDat[i];
	}
	for (loop = 0; loop < Numbers_of_Movie - 1; loop++) {
		for (i = 0; i < Numbers_of_Movie - 1 - loop; i++) {
			if (MovieSortDat[i].CountTag < MovieSortDat[i + 1].CountTag) {
				hold = MovieSortDat[i + 1];
				MovieSortDat[i + 1] = MovieSortDat[i];
				MovieSortDat[i] = hold;
			}
		}
	}

	char genredata[15];
	printf("----------------------- < Genre List > -----------------------\n");
	printf("Action / Adventure / Animation / Children / Comedy / Crime\n");
	printf("Documentary / Drama / Fantasy / Film-Noir / Horror / Musical\n");
	printf("Mystery / Romance / Sci-Fi / Thriller / War\n\n");
	printf("Enter genre: ");
	scanf(" %[^\n]s", genredata);

	for (j = 0; j < Numbers_of_Movie - 1; j++)
	{
		for (k = 0; k < 10; k++)
		{
			if (strcmp(MovieSortDat[j].MovieGenre[k].Genre, genredata) == 0)
			{
				popmovie[count] = j;
				count++;
				if (count == 10)
				{
					for (k = 0; k < 10; k++)
						printf("%d. %s\n", k + 1, MovieSortDat[popmovie[k]].Title);
				}
			}
		}
	}
	if (count == 0)
	{
		printf("This genre does not exist.\n");
	}
}

void Run()
{
	int key = 0;
	while ((key = SelectMenu()) != 0)
	{
		switch (key)
		{
		case 1: FindMovie1(); break;
		case 2: FindMovie2(); break;
		case 3: FindTag(); break;
		case 4: PopularMovie(); break;
		default: printf("No number.\n"); break;
		}
	}
}

int SelectMenu()
{
	printf("---------- << Movie Search Program >> ----------\n");
	printf("1. Find Movies from Tag\n2. Find Movies from Genre\n3. Find Tags from Movie\n4. Popular Movies by Genre\n0. Exit\n");
	printf("Please select a number: ");
	return getnum();
}

int getnum()
{
	int num = 0;
	if (scanf("%d", &num) == 0)
	{
		char buf[256];
		gets_s(buf, sizeof(buf));
	}
	return num;
}