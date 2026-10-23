/*
 * A123020  Expansion of (1 -5*x +5*x^2)/((1 -2*x)*(1 -4*x +x^2)).
 * https://oeis.org/A123020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3) with initial values
 *            a(0)=1, a(1)=1, a(2)=2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds a cache of values to handle calls in any order. */
#include <stdlib.h>
#include <stdio.h>  /* optional, for abort() if needed */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size of cache      */
    static idx_t cache_last = (idx_t)-1; /* highest n computed so far */

    /* Handle the three base terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* make sure the cache is large enough */
    if (cache_cap <= n) {
        /* grow the cache (start with 64 entries, double each time) */
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* memory allocation failure */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* initialise the cache once */
    if (cache_last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_last = 2;
    }

    /* compute any missing terms up to n */
    if (cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t term = cache[i-1] * (seq_t)6; /* 6*a(i-1) */
            term += cache[i-3] * (seq_t)2;      /* + 2*a(i-3) */
            term -= cache[i-2] * (seq_t)9;      /* - 9*a(i-2) */
            cache[i] = term;
        }
        cache_last = n;
    }

    return cache[n];
}
