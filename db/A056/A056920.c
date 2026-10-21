/*
 * A056920  Denominators of continued fraction for left factorial.
 * https://oeis.org/A056920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 25, i64 n <= 41, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) - floor(n/2) * a(n-2)
   Initial values: a(0) = 1, a(1) = 1
   (from OEIS A056920) */

#include <stdlib.h>   /* for realloc, NULL */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static idx_t cacheSize  = 0;   /* currently allocated size */
    static idx_t nextComputed = 2; /* next index yet to be computed */

    /* Ensure the cache is large enough to hold index n */
    if (n >= cacheSize) {
        idx_t newSize = (cacheSize == 0) ? 4 : cacheSize;
        while (newSize <= n) newSize <<= 1;          /* grow by powers of two */

        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                   /* allocation failed */

        cache = tmp;
        /* Initialise the newly added entries to zero */
        for (idx_t i = cacheSize; i < newSize; ++i)
            cache[i] = (seq_t)0;
        cacheSize = newSize;
    }

    /* Initialise the first two sequence values when the cache first becomes
       available (this happens on the first call) */
    if (nextComputed == 2 && cacheSize > 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* If the requested index is already computed, return it immediately */
    if (n < nextComputed)
        return cache[n];

    /* Compute the remaining terms up to n using the recurrence */
    for (idx_t i = nextComputed; i <= n; ++i) {
        seq_t t1    = cache[i - 1];
        seq_t t2    = cache[i - 2];
        seq_t coeff = (seq_t)(i >> 1);        /* floor(i/2) */
        cache[i] = t1 - coeff * t2;
    }

    nextComputed = n + 1;
    return cache[n];
}
