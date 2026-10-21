/*
 * A077894  Expansion of (1-x)^(-1)/(1+2*x^2+x^3).
 * https://oeis.org/A077894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3) + a(n-4) (n ≥ 4) Initial
 *            values: a(0) = 1, a(1) = 1, a(2) = -1, a(3) = -2
 * Range:     i8 n <= 13, i16 n <= 26, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache   = NULL;   /* array of computed terms          */
    static size_t cache_cap = 0;    /* allocated capacity (elements)    */
    static idx_t  cache_len  = 0;   /* number of terms stored           */

    /* Ensure the array can hold indices 0 … n, and at least the first four terms. */
    size_t needed = (size_t)n + 1;
    if (needed < 4) needed = 4;
    if (n >= (idx_t)cache_cap) {          /* grow as necessary */
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* out of memory – abort silently */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first four terms if not yet done. */
    if (cache_len <= 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)-2;
        cache_len = 4;
    }

    /* Compute any missing terms up to n. */
    while (cache_len <= n) {
        idx_t i   = cache_len;
        seq_t val = cache[i-1] 
                  - (seq_t)2 * cache[i-2]
                  + cache[i-3]
                  + cache[i-4];
        cache[i] = val;
        ++cache_len;
    }

    return cache[(size_t)n];
}
