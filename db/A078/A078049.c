/*
 * A078049  Expansion of (1-x)/(1+x+2*x^2-x^3).
 * https://oeis.org/A078049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) + a(n-3), with a(0)=1, a(1)=-2, a(2)=0
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 45, i64 n <= 92, i128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of elements stored in cache */

    /* Initialise cache for the first call */
    if (cached == 0) {
        cached = 3;
        cache  = (seq_t *)malloc(sizeof(seq_t) * cached);
        if (!cache) return 0;                /* allocation failure: fallback */
        cache[0] = (seq_t) 1;   /* a(0) */
        cache[1] = (seq_t)-2;   /* a(1) */
        cache[2] = (seq_t) 0;   /* a(2) */
    }

    /* Return directly if we already know a(n) */
    if (n < cached) {
        return cache[(size_t)n];
    }

    /* Extend cache up to index n */
    idx_t new_limit = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_limit);
    if (!tmp) return 0;                  /* allocation failure: fallback */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t v1 = cache[(size_t)(i - 1)];
        seq_t v2 = cache[(size_t)(i - 2)];
        seq_t v3 = cache[(size_t)(i - 3)];
        /* a(i) = -a(i-1) - 2*a(i-2) + a(i-3) */
        cache[(size_t)i] = -v1 - (seq_t)2 * v2 + v3;
    }
    cached = new_limit;

    return cache[(size_t)n];
}
