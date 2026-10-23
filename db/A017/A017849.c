/*
 * A017849  Expansion of 1/(1-x^6-x^7-x^8-x^9).
 * https://oeis.org/A017849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6)+a(n-7)+a(n-8)+a(n-9); Initial values: a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=1, a(8)=1,
 *            a(9)=1
 * Range:     u8 n <= 40, u16 n <= 69, u32 n <= 128, u64 n <= 246,
 *            u128 n <= 483, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* number of values already filled */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* ensure some initial capacity */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure; fall back to zero */
    }

    /* grow the allocated array if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i < 6)
            val = 0;
        else if (i < 10)
            val = (seq_t)1;
        else
            val = cache[i-6] + cache[i-7] + cache[i-8] + cache[i-9];
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
