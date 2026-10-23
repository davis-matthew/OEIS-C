/*
 * A159451  Number of numbers less than n, that are contained in n written as
 *          English number names.
 * https://oeis.org/A159451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of integers m < n such that the English word of m is
 * a substring of the English word of n.  This implementation
 * generates the words up to n and counts the matches. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Generate the English name of a non‑negative integer, without spaces
 * or hyphens.  The caller must free the returned string.               */
static char *eng_name(idx_t n)
{
    if (n == 0) {
        return strdup("zero");
    }

    /* Temporary buffer – large enough for numbers up to several million. */
    char buffer[128];
    char *p = buffer;

    /* Thousands (recursively). */
    if (n >= 1000) {
        idx_t thousands = n / 1000;
        char *th_str = eng_name(thousands);
        size_t tl = strlen(th_str);
        memcpy(p, th_str, tl);
        p += tl;
        memcpy(p, "thousand", 8);
        p += 8;
        free(th_str);
        n %= 1000;
    }

    /* Hundreds. */
    if (n >= 100) {
        idx_t hundreds = n / 100;
        const char *unit_names[10] = {
            "", "one", "two", "three", "four",
            "five", "six", "seven", "eight", "nine"
        };
        const char *hundred_str = unit_names[hundreds];
        size_t hl = strlen(hundred_str);
        memcpy(p, hundred_str, hl);
        p += hl;
        memcpy(p, "hundred", 7);
        p += 7;
        n %= 100;
    }

    /* Tens 20‑99. */
    if (n >= 20) {
        idx_t tens = n / 10;
        const char *tens_names[10] = {
            "", "", "twenty", "thirty", "forty",
            "fifty", "sixty", "seventy", "eighty", "ninety"
        };
        const char *t_str = tens_names[tens];
        size_t tl = strlen(t_str);
        memcpy(p, t_str, tl);
        p += tl;
        n %= 10;
    }

    /* 10‑19. */
    if (n >= 10) {
        const char *teens[10] = {
            "", "eleven", "twelve", "thirteen", "fourteen",
            "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
        };
        const char *t_str = (n == 10) ? "ten" : teens[n - 10];
        size_t tl = strlen(t_str);
        memcpy(p, t_str, tl);
        p += tl;
        n = 0;
    }

    /* Units 1‑9. */
    if (n > 0) {
        const char *unit_names[10] = {
            "", "one", "two", "three", "four",
            "five", "six", "seven", "eight", "nine"
        };
        const char *u_str = unit_names[n];
        size_t ul = strlen(u_str);
        memcpy(p, u_str, ul);
        p += ul;
    }

    *p = '\0';

    /* Allocate result with exact length. */
    size_t len = (size_t)(p - buffer);
    char *result = (char *)malloc(len + 1);
    if (!result) abort();
    memcpy(result, buffer, len + 1);
    return result;
}

/* --------------------------------------------------------------------- */
/* Static storage for names and a(n) values. */
static char **g_names = NULL;     /* array of strings */
static seq_t *g_a_cache = NULL;   /* array of a(n) values */
static idx_t g_max_cached = 0;    /* highest index currently cached */

/* --------------------------------------------------------------------- */
/* Extend the cache to include all indices up to n. */
static void extend_cache(idx_t n)
{
    /* First call – initialise the arrays. */
    if (g_names == NULL) {
        size_t new_cap = (size_t)n + 1;
        g_names    = (char **)malloc(new_cap * sizeof(*g_names));
        g_a_cache  = (seq_t *)malloc(new_cap * sizeof(*g_a_cache));
        if (!g_names || !g_a_cache) abort();
        g_names[0]    = eng_name(0);          /* "zero" */
        g_a_cache[0]  = (seq_t)0;             /* a(0) = 0 */
        g_max_cached  = 0;
    }

    if (n <= g_max_cached) {
        return;  /* nothing to do */
    }

    /* Resize the arrays. */
    size_t new_cap = (size_t)n + 1;
    char **tmp_names = (char **)realloc(g_names, new_cap * sizeof(*g_names));
    seq_t *tmp_cache = (seq_t *)realloc(g_a_cache, new_cap * sizeof(*g_a_cache));
    if (!tmp_names || !tmp_cache) abort();
    g_names   = tmp_names;
    g_a_cache = tmp_cache;

    /* Compute new entries. */
    for (idx_t i = g_max_cached + 1; i <= n; ++i) {
        g_names[i] = eng_name(i);
        size_t cnt = 0;
        const char *name_i = g_names[i];
        for (idx_t m = 0; m < i; ++m) {
            if (strstr(name_i, g_names[m])) {
                ++cnt;
            }
        }
        g_a_cache[i] = (seq_t)cnt;
    }

    g_max_cached = n;
}

/* --------------------------------------------------------------------- */
/* Public sequence function required by the build. */
seq_t a(idx_t n)
{
    extend_cache(n);
    return g_a_cache[n];
}
