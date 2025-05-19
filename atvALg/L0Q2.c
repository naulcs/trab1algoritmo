#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINE 1000
#define MAX_TOKEN 100

typedef struct
{
    double x, y;
} Ponto;

double dist(Ponto p)
{
    return sqrt(p.x * p.x + p.y * p.y);
}

int is_float(const char *s)
{
    char *end;
    strtod(s, &end);
    return *end == '\0' && strchr(s, '.') != NULL;
}

int is_int(const char *s)
{
    char *end;
    strtol(s, &end, 10);
    return *end == '\0' && strchr(s, '.') == NULL;
}

int is_point(const char *s, Ponto *out)
{
    return sscanf(s, " ( %lf , %lf ) ", &out->x, &out->y) == 2;
}

int cmp_str(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int cmp_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int cmp_double(const void *a, const void *b)
{
    double diff = *(double *)a - *(double *)b;
    return (diff > 0) - (diff < 0);
}

int cmp_point(const void *a, const void *b)
{
    double da = dist(*(Ponto *)a);
    double db = dist(*(Ponto *)b);
    return (da > db) - (da < db);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s arquivo.in\n", argv[0]);
        return 1;
    }

    char in_name[256];
    char out_name[256];

    strncpy(in_name, argv[1], 255);
    strcpy(out_name, argv[1]);
    char *dot = strrchr(out_name, '.');
    if (dot)
        strcpy(dot, ".out");

    FILE *fin = fopen(in_name, "r");
    FILE *fout = fopen(out_name, "w");
    if (!fin || !fout)
    {
        fprintf(stderr, "Erro ao abrir arquivos\n");
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, fin))
    {
        char *tokens[MAX_TOKEN];
        int int_list[MAX_TOKEN], int_count = 0;
        double float_list[MAX_TOKEN];
        int float_count = 0;
        char *str_list[MAX_TOKEN];
        int str_count = 0;
        Ponto point_list[MAX_TOKEN];
        int point_count = 0;

        int n = 0;
        char *tok = strtok(line, " \n");
        while (tok && n < MAX_TOKEN)
        {
            Ponto p;
            if (is_point(tok, &p))
            {
                point_list[point_count++] = p;
            }
            else if (is_int(tok))
            {
                int_list[int_count++] = atoi(tok);
            }
            else if (is_float(tok))
            {
                float_list[float_count++] = atof(tok);
            }
            else
            {
                str_list[str_count] = malloc(strlen(tok) + 1);
                strcpy(str_list[str_count++], tok);
            }
            tok = strtok(NULL, " \n");
        }

        // Ordenar
        qsort(str_list, str_count, sizeof(char *), cmp_str);
        qsort(int_list, int_count, sizeof(int), cmp_int);
        qsort(float_list, float_count, sizeof(double), cmp_double);
        qsort(point_list, point_count, sizeof(Ponto), cmp_point);

        // Escrever saída
        fprintf(fout, "str:");
        for (int i = 0; i < str_count; i++)
        {
            fprintf(fout, "%s%s", i ? " " : "", str_list[i]);
            free(str_list[i]);
        }

        fprintf(fout, " int:");
        for (int i = 0; i < int_count; i++)
            fprintf(fout, "%s%d", i ? " " : "", int_list[i]);

        fprintf(fout, " float:");
        for (int i = 0; i < float_count; i++)
            fprintf(fout, "%s%.10g", i ? " " : "", float_list[i]);

        fprintf(fout, " p:");
        for (int i = 0; i < point_count; i++)
            fprintf(fout, "%s(%.10g,%.10g)", i ? " " : "", point_list[i].x, point_list[i].y);

        fprintf(fout, "\n");
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
