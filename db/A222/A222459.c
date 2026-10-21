/*
 * A222459  Number of n X 3 0..7 arrays with no entry increasing mod 8 by 7
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 49, a(2) = 12943, a(n) = 270*a(n-1) - 1547*a(n-2) (n ≥ 3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1, harmless */

    static seq_t *cache   = NULL;        /* 1‑based array: cache[i] = a(i) */
    static idx_t  cache_len = 0;        /* highest n that has been computed */
    static idx_t  cache_cap = 0;        /* current allocated capacity (>= cache_len+1) */

    /* Initial allocation (ensure space for indices 1 and 2) */
    if (!cache) {
        idx_t cap = (n < 2 ? 3 : n + 1); /* at least 3 to hold a(1) and a(2) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = cap;
        cache[1] = (seq_t)49;            /* a(1) */
        cache[2] = (seq_t)12943;         /* a(2) */
        cache_len = 2;
    }

    /* Extend allocation if the requested n is larger than current capacity */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 3) new_cap = 3;   /* safety for very small n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    if (cache_len < n) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = (seq_t)270 * cache[i-1] - (seq_t)1547 * cache[i-2];
        }
        cache_len = n;
    }

    return cache[n];
}
