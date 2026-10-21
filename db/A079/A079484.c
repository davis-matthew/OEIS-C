/*
 * A079484  a(n) = (2n-1)!! * (2n+1)!!, where the double factorial is A006882.
 * https://oeis.org/A079484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^2 - 1) * a(n-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"   /* mul_small(seq_t, unsigned int) */
#include "../utils/alloc_seq_array.h" /* alloc_seq_array(size_t) */
#include "../utils/grow_seq_array.h" /* grow_seq_array(seq_t**, size_t*, size_t) */

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache */
    static size_t cache_len = 0;   /* number of stored terms */

    /* Base case: a(0) = 1 */
    if (n == 0) {
        if (cache == NULL) {
            /* initialise cache with a(0) */
            cache_cap = 1;
            cache = alloc_seq_array(cache_cap);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Initialise cache if not done yet */
    if (cache == NULL) {
        cache_cap = 1;
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Ensure the cache can hold terms up to n */
    if ((size_t)n + 1 > cache_cap) {
        grow_seq_array(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Compute missing terms using the recurrence */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        /* factor = 4*k*k - 1  (fits in a 32‑bit unsigned int for all n <= 10000) */
        uint64_t factor = 4ULL * k * k - 1ULL;
        /* Multiply previous term by factor using mul_small to keep arithmetic cheap */
        cache[k] = mul_small(cache[k - 1], (unsigned int)factor);
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
