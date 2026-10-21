/*
 * A202879  E.g.f.: exp(4*x + x^2/2).
 * https://oeis.org/A202879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(n)=4*a(n-1)+(n-1)*a(n-2) for n>=2 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of valid entries in cache */
    static size_t cache_cap  = 0;  /* capacity of cache */

    if (n >= cache_size) {
        /* make sure the cache is large enough */
        ensure_cache(n, &cache, &cache_cap);

        /* compute and store missing entries */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)4;
            } else {
                seq_t term1 = (seq_t)4 * cache[i - 1];
                seq_t term2 = (seq_t)(i - 1) * cache[i - 2];
                cache[i] = term1 + term2;
            }
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
