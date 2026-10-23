/*
 * A147839  a(n)=9*a(n-1)-7*a(n-2), a(0)=1, a(1)=7 .
 * https://oeis.org/A147839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 7*a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_last = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* out‑of‑memory: error code */
        cache[0] = 1;
        cache[1] = 7;
        cache_last = 1;
    }

    /* return cached value if already computed */
    if (n <= cache_last)
        return cache[n];

    /* grow the cache array if necessary */
    if (n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;               /* out‑of‑memory */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute missing terms */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = (seq_t)9 * cache[i - 1] - (seq_t)7 * cache[i - 2];
    }
    cache_last = n;

    return cache[n];
}
