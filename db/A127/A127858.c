/*
 * A127858  Positive integers n such that r(n^2)=r(n)^2, where r is the cyclic
 *          replacement map of the digits d of n in base 12, that is, d->d+1
 *          if d<11 and d->0 if d=11.
 * https://oeis.org/A127858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 12*a(n-1) - 6, a(1) = 6 (derived from 5*(12^n-1)/11+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard against undefined behaviour for n == 0 (should never happen) */
    if (n == 0) return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Grow cache if needed */
    if (n > cache_len) {
        /* Allocate or extend array to hold values up to n */
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – return 0 as a sentinel */
            return 0;
        }
        cache = new_cache;

        /* Initialise from the first cached element if necessary */
        if (cache_len == 0) {
            cache[0] = (seq_t)6; /* a(1) = 6 */
            cache_len = 1;
        }

        /* Compute missing terms */
        for (idx_t i = cache_len; i < n; i++) {
            /* Compute a(i+1) = 12*a(i) - 6 using a wider type to avoid overflow
               when seq_t is 64‑bit or smaller. For larger types the cast is
               a no‑op. */
            unsigned __int128 tmp = (unsigned __int128)cache[i - 1] * 12ull;
            tmp -= 6ull;
            cache[i] = (seq_t)tmp;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
