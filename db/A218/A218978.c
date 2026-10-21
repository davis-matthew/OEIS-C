/*
 * A218978  Table read by rows: n-th row lists all distinct substrings of
 *          decimal representation of n.
 * https://oeis.org/A218978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 1138, u16 n <= 5315, u32 n <= 5315, u64 n <= 5315,
 *            u128 n <= 5315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Computes OEIS A218978.
 *  a(n) = the n-th element of the flattened table whose
 *  row m lists all distinct numeric substrings of the decimal
 *  representation of m, sorted increasingly.  The table is read
 *  by rows, starting with row 0, and each row contributes its
 *  sorted list of distinct substrings to the global sequence
 *  in that order.
 */

/* Maximum digits we may encounter for any idx_t (64‑bit yields ≤20,
 *  128‑bit yields ≤39).  40 is a safe upper bound.  */
#define MAX_DIGITS 40
/* Worst case number of distinct substrings in a row. */
#define MAX_SUBS ((MAX_DIGITS * (MAX_DIGITS + 1)) / 2)

/* Comparison routine for qsort on seq_t values. */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* Global cache of sequence values. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* filled entries */
static idx_t   next_row  = 0;   /* next natural number to process */

/* Ensure the cache has room for at least `need` elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate row for integer `m` and append its distinct substrings
 * (sorted ascending) to the global cache. */
static void generate_row(idx_t m)
{
    /* Extract decimal digits of `m` into array `digits`. */
    unsigned char digits[MAX_DIGITS];
    size_t len = 0;

    if (m == (idx_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = m;
        while (tmp > (idx_t)0) {
            digits[len++] = (unsigned char)(tmp % (idx_t)10);
            tmp /= (idx_t)10;
        }
        /* Reverse to correct order: most‑significant digit first. */
        for (size_t i = 0; i < len / 2; ++i) {
            unsigned char t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* Enumerate all numeric substrings and store them in a local array. */
    seq_t subs[MAX_SUBS];
    size_t subs_len = 0;
    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * (seq_t)10 + (seq_t)digits[j];
            subs[subs_len++] = val;
        }
    }

    /* Sort collected substrings and keep only distinct values. */
    qsort(subs, subs_len, sizeof(seq_t), cmp_seq);

    for (size_t i = 0; i < subs_len; ++i) {
        if (i == 0 || subs[i] != subs[i - 1]) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = subs[i];
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Keep generating rows until we have at least (n + 1) terms. */
    while (cache_len <= (size_t)n) {
        generate_row(next_row++);
    }
    return cache[(size_t)n];
}
