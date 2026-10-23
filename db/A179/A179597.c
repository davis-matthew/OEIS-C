/*
 * A179597  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + 5*x + 2*x^2)/(1 - 2*x - 11*x^2 - 6*x^3).
 * https://oeis.org/A179597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 11*a(n-2) + 6*a(n-3), with a(0)=1, a(1)=7,
 *            a(2)=27
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid entries in cache */
    static size_t cache_cap  = 0;   /* allocated capacity of cache */

    /* Ensure the cache exists with at least 3 entries for the initial values */
    if (cache == NULL) {
        size_t init_cap = 3;
        /* Expand capacity to cover n if needed */
        if ((size_t)n + 1 > init_cap) {
            while (init_cap <= (size_t)n) init_cap <<= 1;
        }
        /* Allocate or reallocate cache */
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[init_cap];
        cache = new_cache;
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        cache_cap = init_cap;
        /* Set initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)27;
        cache_size = 3;
    }
    /* Grow cache if necessary for larger n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        delete [] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }
    /* If the desired term is already computed, return it directly */
    if ((size_t)n < cache_size) {
        return cache[n];
    }
    /* Compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)2
                   + cache[i - 2] * (seq_t)11
                   + cache[i - 3] * (seq_t)6;
        cache[i] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
