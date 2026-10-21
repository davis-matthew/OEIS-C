/*
 * A080881  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0)=1, a(1)=2, a(2)=10.
 * https://oeis.org/A080881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 11*a(n+2) - 4*a(n), a(0)=1, a(1)=2, a(2)=10, a(3)=21
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms already computed */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    /* make sure the cache has room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[(size_t)i] = (seq_t)1;
        else if (i == 1)     cache[(size_t)i] = (seq_t)2;
        else if (i == 2)     cache[(size_t)i] = (seq_t)10;
        else if (i == 3)     cache[(size_t)i] = (seq_t)21;
        else                 cache[(size_t)i] = cache[(size_t)(i-2)] * (seq_t)11
                                            - cache[(size_t)(i-4)] * (seq_t)4;
    }

    /* update length of computed terms */
    if ((size_t)(n + 1) > cache_len) cache_len = (size_t)(n + 1);

    return cache[(size_t)n];
}
