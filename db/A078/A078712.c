/*
 * A078712  Series expansion of (-3 - 2*x)/(1 + x - x^3) in powers of x.
 * https://oeis.org/A078712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-3) - a(n-1) with a(0) = -3, a(1) = 1, a(2) = -1
 * Range:     i8 n <= 30, i16 n <= 70, i32 n <= 150, i64 n <= 307,
 *            i128 n <= 623, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of slots allocated */
    static idx_t max_computed = 0;/* largest index that has been computed */

    /* Handle the base cases directly */
    if (n < 3) {
        switch (n) {
        case 0: return (seq_t)-3;
        case 1: return (seq_t)1;
        case 2: return (seq_t)-1;
        }
    }

    /* First call: create an initial cache and set the base values */
    if (cache == NULL) {
        /* Allocate enough space for the requested index */
        size_t init_cap = (size_t)((n < 64) ? 64 : n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!cache) return (seq_t)0; /* fallback if allocation fails */
        cache[0] = (seq_t)-3;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        max_computed = 2;
        cache_cap = init_cap;
    }

    /* If needed, grow the cache to accommodate the requested index */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* fallback on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i - 3] - cache[i - 1];
        }
        max_computed = n;
    }

    return cache[n];
}
