/*
 * A091091  Numbers needing in binary and ternary representation an equal
 *          minimal number of editing steps (delete, insert or substitute) to
 *          transform them into their square.
 * https://oeis.org/A091091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer x such that the Levenshtein distance
 *            between the representation of x and its square is equal in base
 *            2 and base 3
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------------------------------------------------------- */
/* Basic headers -------------------------------------------------------- */
#include <stdbool.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------- */
/* Constants ------------------------------------------------------------ */
#define MAX_DIGITS 512   /* enough for base‑2/3 digits up to the required range */

/* ---------------------------------------------------------------------- */
/* Helper: convert a number to base 'base', most significant digit first   */
/* (digits are stored in buf[], which must have space for at least          */
/* MAX_DIGITS entries).  Returns the number of digits written.              */
static int to_base_digits(seq_t num, int base, unsigned char *buf)
{
    int len = 0;
    if (num == 0) {
        buf[0] = 0;
        return 1;
    }
    /* collect digits least significant first */
    while (num != 0) {
        buf[len++] = (unsigned char)(num % base);
        num /= base;
        if (len >= MAX_DIGITS) break;   /* safety guard */
    }
    /* reverse to most significant first */
    for (int i = 0; i < len / 2; ++i) {
        unsigned char tmp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = tmp;
    }
    return len;
}

/* ---------------------------------------------------------------------- */
/* Helper: Levenshtein (edit) distance between two digit arrays           */
/* ---------------------------------------------------------------------- */
static int edit_distance(const unsigned char *a, int a_len,
                         const unsigned char *b, int b_len)
{
    static int prev[MAX_DIGITS + 1];
    static int curr[MAX_DIGITS + 1];

    for (int j = 0; j <= b_len; ++j) prev[j] = j;
    for (int i = 1; i <= a_len; ++i) {
        curr[0] = i;
        unsigned char ai = a[i - 1];
        for (int j = 1; j <= b_len; ++j) {
            int cost = (ai == b[j - 1]) ? 0 : 1;
            int del = prev[j] + 1;
            int ins = curr[j - 1] + 1;
            int sub = prev[j - 1] + cost;
            int best = del;
            if (ins < best) best = ins;
            if (sub < best) best = sub;
            curr[j] = best;
        }
        for (int j = 0; j <= b_len; ++j) prev[j] = curr[j];
    }
    return prev[b_len];
}

/* ---------------------------------------------------------------------- */
/* Dynamic cache for the sequence values                                  */
/* ---------------------------------------------------------------------- */
static seq_t   *cache = NULL;  /* array of found terms         */
static idx_t    cache_cap = 0; /* allocated capacity of cache   */
static idx_t    cache_len = 0; /* number of terms stored        */
static idx_t    next_candidate = 0; /* next integer to test      */

/* ---------------------------------------------------------------------- */
/* Main sequence function                                                  */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we reach the n-th one */
    while (cache_len < n) {
        seq_t x  = (seq_t)next_candidate;
        seq_t sq = x * x;                 /* square of current number */

        unsigned char binX[MAX_DIGITS], binSq[MAX_DIGITS];
        int lenX  = to_base_digits(x, 2, binX);
        int lenSq = to_base_digits(sq, 2, binSq);
        int distBin = edit_distance(binX, lenX, binSq, lenSq);

        unsigned char terX[MAX_DIGITS], terSq[MAX_DIGITS];
        int lenTX  = to_base_digits(x, 3, terX);
        int lenTQ  = to_base_digits(sq, 3, terSq);
        int distTer = edit_distance(terX, lenTX, terSq, lenTQ);

        if (distBin == distTer) {
            /* store term, expanding cache if needed */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = x;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
