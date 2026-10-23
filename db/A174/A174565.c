/*
 * A174565  Expansion of g.f. (1+3*x)/((1-x)*(1+3*x+4*x^2)).
 * https://oeis.org/A174565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 31, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A174565: Coefficients of (1+3*x)/((1-x)*(1+3*x+4*x^2));
    Recurrence:  a(n) + 2*a(n-1) + a(n-2) - 4*a(n-3) = 0   (n >= 3)
    Initial values:  a(0) = 1,  a(1) = 1,  a(2) = -3   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache       = NULL;
    static idx_t cache_size   = 0;   /* number of entries currently stored */
    static idx_t cache_cap    = 0;   /* allocated capacity */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-3;
        cache_size = 3;
    }

    /* return already computed value */
    if (n < cache_size)
        return cache[n];

    /* grow the cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;                     /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    idx_t start = cache_size;   /* first index not yet computed */
    if (start < 3) start = 3;    /* safety, though should already be 3 */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = - (seq_t)2 * cache[i-1]   /* -2*a(n-1)  */
                 - cache[i-2]                /* -a(n-2)    */
                 + (seq_t)4 * cache[i-3];    /* +4*a(n-3)  */
    }

    cache_size = n + 1;
    return cache[n];
}
