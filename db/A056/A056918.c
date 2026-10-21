/*
 * A056918  a(n) = 9*a(n-1)-a(n-2); a(0)=2, a(1)=9.
 * https://oeis.org/A056918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - a(n-2), a(0) = 2, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized computation using an expandable cache */
seq_t a(idx_t n)
{
    /* static cache pointing to computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cached entries */

    /* Initialize cache with the first two terms if not yet done */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed: return 0 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
    }

    /* If the requested index is already cached, return it immediately */
    if (n < cache_size) {
        return cache[n];
    }

    /* Expand the cache to accommodate index n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache,
                           (size_t)new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;           /* allocation failed */
    cache = new_cache;

    /* Compute missing terms iteratively */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 9*a(i-1) - a(i-2) */
        cache[i] = 9 * cache[i - 1] - cache[i - 2];
    }
    cache_size = new_size;

    return cache[n];
}
