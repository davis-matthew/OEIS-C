/*
 * A077835  Expansion of 1/(1 - 2*x - 2*x^2 - 2*x^3).
 * https://oeis.org/A077835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) with a(0)=1, a(1)=2,
 *            a(2)=6 This recurrence follows from 1/(1 - 2*x - 2*x^2 - 2*x^3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initialise table with the first three terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache_len = 3;
    }

    /* if n already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* grow capacity until it can hold index n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + 2*a(i-2) + 2*a(i-3) */
        cache[i] = (seq_t)(2 * cache[i-1] + 2 * cache[i-2] + 2 * cache[i-3]);
    }
    cache_len = n + 1;

    return cache[n];
}
