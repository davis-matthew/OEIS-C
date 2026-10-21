/*
 * A214095  Position of integer n in the list of first n natural numbers
 *          written out in French and arranged in alphabetical order.
 * https://oeis.org/A214095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, word
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   cmp_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A214095: position of integer n in the alphabetically
   sorted list of the first n natural numbers written in French (ignoring
   hyphens and spaces). */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/cmp_word.h"

/* ------------------------------------------------------------------ */
/* French number to string conversion                                 */
/* ------------------------------------------------------------------ */

static const char *UNITS[] = {
    "", "un", "deux", "trois", "quatre", "cinq", "six", "sept",
    "huit", "neuf", "dix", "onze", "douze", "treize",
    "quatorze", "quinze", "seize"
};

static const char *TENS[] = {
    "", "", "vingt", "trente", "quarante", "cinquante", "soixante",
    "soixante", "quatre-vingt", "quatre-vingt"
};

static size_t add_str(char *buf, size_t pos, const char *s)
{
    size_t l = strlen(s);
    memcpy(buf + pos, s, l);
    return pos + l;
}

static size_t write_recursive(idx_t n, char *buf, size_t pos)
{
    if (n < 17) {
        return add_str(buf, pos, UNITS[n]);
    }

    if (n < 20) {
        pos = add_str(buf, pos, "dix-");
        return add_str(buf, pos, UNITS[n - 10]);
    }

    if (n < 70) {                     /* 20‑69 */
        int ten = n / 10;
        int rem = n % 10;
        pos = add_str(buf, pos, TENS[ten]);

        if (rem == 0) return pos;
        if (rem == 1) return add_str(buf, pos, " et un");

        pos = add_str(buf, pos, "-");
        return add_str(buf, pos, UNITS[rem]);
    }

    if (n < 80) {                     /* 70‑79 */
        int rest = n - 60;            /* 10‑19 */
        if (rest == 11)              /* "soixante et onze" */
            return add_str(buf, pos, "soixante et onze");
        pos = add_str(buf, pos, "soixante-");
        return write_recursive(rest, buf, pos);
    }

    if (n < 100) {                    /* 80‑99 */
        if (n == 80)
            return add_str(buf, pos, "quatre-vingts");
        pos = add_str(buf, pos, "quatre-vingt-");
        return write_recursive(n - 80, buf, pos);
    }

    if (n < 1000) {                   /* 100‑999 */
        int h = n / 100;
        int r = n % 100;
        if (r == 0) {
            if (h == 1)
                return add_str(buf, pos, "cent");
            pos = add_str(buf, pos, UNITS[h]);
            return add_str(buf, pos, " cents");
        } else {
            if (h == 1)
                pos = add_str(buf, pos, "cent ");
            else {
                pos = add_str(buf, pos, UNITS[h]);
                pos = add_str(buf, pos, " cent ");
            }
            return write_recursive(r, buf, pos);
        }
    }

    if (n < 1000000) {                /* 1 000‑999 999 */
        int t = n / 1000;
        int r = n % 1000;
        if (t == 1)
            pos = add_str(buf, pos, "mille");
        else {
            pos = write_recursive(t, buf, pos);
            pos = add_str(buf, pos, " mille");
        }
        if (r == 0) return pos;
        pos = add_str(buf, pos, " ");
        return write_recursive(r, buf, pos);
    }

    /* fallback: numeric string (never used for n ≤ 10000) */
    return snprintf(buf + pos, 200 - pos, "%llu",
                    (unsigned long long)n);
}

static char *french_word(idx_t n)
{
    char tmp[256];
    size_t pos = write_recursive(n, tmp, 0);
    tmp[pos] = '\0';
    char *res = (char *)malloc(pos + 1);
    memcpy(res, tmp, pos + 1);
    return res;
}

/* ------------------------------------------------------------------ */
/* Helper: compare two words ignoring spaces and hyphens              */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Memoisation storage                                                */
/* ------------------------------------------------------------------ */

static idx_t max_n = 0;          /* largest n processed so far */
static size_t *sorted = NULL;    /* indices sorted by word */
static size_t sorted_cap = 0;
static idx_t *rank_arr = NULL;   /* rank of each number when computed */
static size_t rank_cap = 0;
static char **words = NULL;      /* cached words for each number */
static size_t words_cap = 0;

/* ensure capacity for arrays that grow with n */
static void ensure_cap(idx_t n)
{
    if (n > rank_cap) {
        rank_cap = n + 1;
        rank_arr = (idx_t *)realloc(rank_arr, rank_cap * sizeof(idx_t));
    }
    if (n > words_cap) {
        words_cap = n + 1;
        words = (char **)realloc(words, words_cap * sizeof(char *));
    }
    if (n > sorted_cap) {
        sorted_cap = n;
        sorted = (size_t *)realloc(sorted, sorted_cap * sizeof(size_t));
    }
}

/* binary search insertion into sorted array using cmp_word */
static size_t insert_sorted(idx_t idx, const char *word)
{
    size_t lo = 0, hi = max_n;      /* hi is number of elements yet */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        int cmp = cmp_word(word, words[sorted[mid]]);
        if (cmp < 0)
            hi = mid;
        else
            lo = mid + 1;
    }
    /* shift right to make space */
    memmove(sorted + lo + 1, sorted + lo, (max_n - lo) * sizeof(size_t));
    sorted[lo] = idx;
    max_n++;
    return lo + 1; /* 1‑based rank */
}

/* ------------------------------------------------------------------ */
/* Public function                                                   */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 */

    if (n > max_n) {
        ensure_cap(n);
        for (idx_t k = max_n + 1; k <= n; ++k) {
            char *w = french_word(k);
            words[k] = w;
            rank_arr[k] = insert_sorted(k, w);
        }
        max_n = n;
    }
    return (seq_t)rank_arr[n];
}
