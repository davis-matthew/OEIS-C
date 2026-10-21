/*
 * A077853  Expansion of (1-x)^(-1)/(1-2*x+2*x^3).
 * https://oeis.org/A077853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 24, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077853
   Recurrence: a(0)=1, a(1)=3, a(2)=7, a(3)=13
               a(n)=3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 2*a(n-4)  (n≥4)   */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms stored   */
    static size_t cache_cap = 0;         /* allocated array capacity */

    /* Helper to expand the cache up to the requested index */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise first four terms on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)13;
        cache_len = 4;
    }

    /* Extend cache if necessary */
    if ((idx_t)cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t = 3 * cache[i - 1]          /* 3*a(n-1)  */
                    - 2 * cache[i - 2]          /* -2*a(n-2) */
                    - 2 * cache[i - 3]          /* -2*a(n-3) */
                    + 2 * cache[i - 4];         /* +2*a(n-4) */
            cache[i] = t;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
