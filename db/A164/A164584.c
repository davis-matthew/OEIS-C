/*
 * A164584  Expansion of (1 + 6*x - 12*x^2 - 8*x^3)/(1 - 24*x^2 + 16*x^4).
 * https://oeis.org/A164584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-2) - 16*a(n-4) for n ≥ 4 Initial values: a(0)=1,
 *            a(1)=6, a(2)=12, a(3)=136
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   deallocate_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches all computed values up to the largest
   index requested, so that any order of calls is handled efficiently. */

#include <stdlib.h>
#include "../utils/deallocate_cache.h"

static seq_t *allocate_cache(size_t new_size) {
#if defined(__cplusplus)
    return new seq_t[new_size]();
#else
    return (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements allocated */

    /* initialise cache with the first four terms if needed */
    if (cache == NULL) {
        cache = allocate_cache(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)136;
        cache_size = 4;
    }

    /* grow cache if necessary */
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = allocate_cache(new_size);

        /* copy old values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* compute new terms using the linear recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            /* recurrence: a(i)=24*a(i-2)-16*a(i-4) */
            new_cache[i] = new_cache[i - 2] * (seq_t)24 - new_cache[i - 4] * (seq_t)16;
        }

        deallocate_cache(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
