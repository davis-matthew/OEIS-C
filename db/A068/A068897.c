/*
 * A068897  Squares containing 2k digits in which the sum of the first k
 *          digits = that of the rest.
 * https://oeis.org/A068897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   sequence of squares with an even number of digits, where the sum
 *            of the first half of the digits equals the sum of the second
 *            half. The terms are returned in increasing order
 * Range:     u8 none, u16 n <= 1, u32 n <= 1752, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Helper: test whether a square satisfies the property. */
static bool good_square(seq_t sq)
{
    /* Extract digits (least significant first). */
    seq_t tmp = sq;
    int digits[64];          /* enough for 128‑bit numbers */
    int d = 0;
    while (tmp > (seq_t)0) {
        digits[d++] = (int)(tmp % (seq_t)10);
        tmp = tmp / (seq_t)10;
    }
    if (d == 0) return false;          /* 0 is not considered */
    if (d & 1) return false;           /* odd number of digits */
    int half = d >> 1;
    int sum_first = 0, sum_second = 0;
    for (int i = 0; i < half; ++i)      /* least significant half */
        sum_second += digits[i];
    for (int i = half; i < d; ++i)      /* most significant half */
        sum_first += digits[i];
    return sum_first == sum_second;
}

/* Dynamically grow the cache of terms. */
static void grow_terms(size_t *len, size_t *cap, seq_t **arr)
{
    if (*cap == 0) { *cap = 64; *arr = (seq_t*)malloc(*cap * sizeof(seq_t)); }
    else if (*len == *cap) {
        *cap <<= 1;
        *arr = (seq_t*)realloc(*arr, *cap * sizeof(seq_t));
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_root = 1;            /* next integer whose square to test */

    /* Expand cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t root = (seq_t)next_root;
        seq_t sq = root * root;
        if (good_square(sq)) {
            grow_terms(&cache_len, &cache_cap, &cache);
            cache[cache_len++] = sq;
        }
        ++next_root;
    }
    return cache[n - 1];
}
