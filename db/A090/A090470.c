/*
 * A090470  Expansion of e.g.f.: 1/((1-4*x)*sqrt(1-2*x)).
 * https://oeis.org/A090470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n-1)*a(n-1) - 4*(2*n-1)*(n-1)*a(n-2) for n>=2 with
 *            initial values a(0)=1, a(1)=5. This is a homogeneous recurrence
 *            derived from the e.g.f. 1/((1-4x)*sqrt(1-2x)). The function
 *            caches computed terms so that subsequent calls (in any order)
 *            are O(1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* array holding computed a(k) */
    static size_t cache_size = 0;        /* number of valid entries in cache */

    /* initialise cache with base terms */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;            /* a(0) */
        cache[1] = (seq_t)5;            /* a(1) */
    }

    /* extend cache if requested index is beyond current size */
    if ((size_t)n + 1 > cache_size) {
        size_t required = (size_t)n + 1;
        if (required > cache_size * 2) required = cache_size * 2;
        cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        for (size_t i = cache_size; i < required; ++i) cache[i] = 0;
        /* compute missing terms up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = (6*i-1)*a(i-1) - 4*(2*i-1)*(i-1)*a(i-2) */
            seq_t term1 = (seq_t)( (uint64_t)6 * i - 1 ) * cache[i - 1];
            seq_t term2 = (seq_t)( 4 * ( (uint64_t)2 * i - 1 ) * (i - 1) ) * cache[i - 2];
            cache[i] = term1 - term2;
        }
        cache_size = required;
    }

    return cache[(size_t)n];
}
