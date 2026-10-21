/*
 * A226503  Expansion of g.f. x*(1+x+x^2)/(1-x^3-x^5).
 * https://oeis.org/A226503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-5), with initial values a(1) = a(2) = a(3) =
 *            a(4) = a(5) = 1
 * Range:     u8 n <= 34, u16 n <= 65, u32 n <= 128, u64 n <= 253,
 *            u128 n <= 504, big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A226503.
   It uses a static cache to memoise computed terms.
   The cache is grown only when a larger index is requested,
   so repeated calls are handled efficiently. */
#include "../utils/alloc_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1..cache_cap] holds computed values */
    static idx_t  cache_cap = 0;  /* largest index currently cached */

    if (n <= 5)                /* base values */
        return 1;

    /* Ensure the cache can hold indices up to n */
    if (n > cache_cap) {
        const idx_t new_cap = n;
        /* allocate array of size new_cap+1; we ignore index 0 */
        seq_t *new_cache = alloc_cache(new_cap + 1);
        /* initialise all entries to zero */
        for (idx_t i = 1; i <= new_cap; ++i)
            new_cache[i] = 0;
        /* copy previously cached values */
        if (cache) {
            for (idx_t i = 1; i <= cache_cap; ++i)
                new_cache[i] = cache[i];
        }
        /* initialise base values if they hadn't been set yet */
        if (cache_cap < 5) {
            for (idx_t i = 1; i <= 5; ++i)
                new_cache[i] = 1;
        }
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Ensure base values exist (for the initial allocation case) */
    if (cache[1] == 0)
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = 1;

    /* Compute missing terms up to n */
    for (idx_t i = 6; i <= n; ++i) {
        if (cache[i] == 0)
            cache[i] = cache[i - 3] + cache[i - 5];
    }

    return cache[n];
}
