/*
 * A174761  x-values in the solution to x^2-59*y^2=1.
 * https://oeis.org/A174761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1060*a(n-1) - a(n-2) with a(1)=1, a(2)=530
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* sequence is undefined for n<1 */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* array indexed from 1 */
    static idx_t cache_start = 1;   /* index of first stored value */
    static idx_t cache_end   = 0;   /* index of last stored value */
    static size_t cache_cap  = 0;   /* capacity in elements (including index 0) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4; /* minimal space for indices 0..3 */
        cache = (seq_t*)malloc(cache_cap * sizeof(*cache));
        /* set known initial values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)530;
        cache_start = 1;
        cache_end   = 2;
    }

    /* Ensure cache has room up to required index n */
    if (n > cache_end) {
        /* Grow cache if necessary */
        if (n >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap <= n) new_cap *= 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(*cache));
            cache_cap = new_cap;
        }

        /* Compute missing terms */
        for (idx_t i = cache_end + 1; i <= n; ++i) {
            seq_t val = (seq_t)1060 * cache[i - 1] - cache[i - 2];
            cache[i] = val;
        }
        cache_end = n;
    }

    return cache[n];
}
