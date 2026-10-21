/*
 * A077945  Expansion of 1/(1-2*x+2*x^2+2*x^3).
 * https://oeis.org/A077945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) - 2*a(n-3) with a(0)=1, a(1)=2,
 *            a(2)=2
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 34, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* needed for type definitions */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_used = 0;

    /* Initialise cache with first three terms if necessary */
    if (cache_used == 0) {
        cache_cap  = 8;
        cache      = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        cache_used = 3;
    }

    /* If requested index already in cache, return it */
    if ((size_t)n < cache_used)
        return cache[n];

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 8;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute terms up to n using recurrence */
    for (size_t i = cache_used; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        cache[i] = (seq_t)2 * t1 - (seq_t)2 * t2 - (seq_t)2 * t3;
    }
    cache_used = (size_t)n + 1;
    return cache[n];
}
