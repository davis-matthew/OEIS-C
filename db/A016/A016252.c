/*
 * A016252  Expansion of 1/((1-x)*(1-7x)*(1-10x)).
 * https://oeis.org/A016252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 70*a(n-2) + 1; a(0)=1, a(1)=18;
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;
    static int initialized = 0;

    /* Initialise with base terms on first call. */
    if (!initialized) {
        /* Ensure space for indices 0 and 1. */
        ensure_cache(1, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        last_computed = 1;
        initialized = 1;
    }

    /* If a larger index is requested, grow the cache and compute forward. */
    if (n > last_computed) {
        ensure_cache(n, &cache, &cache_cap);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val = 17 * cache[i - 1] - 70 * cache[i - 2] + (seq_t)1;
            cache[i] = val;
        }
        last_computed = n;
    }

    return cache[n];
}
