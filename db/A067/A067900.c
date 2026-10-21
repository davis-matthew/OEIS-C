/*
 * A067900  a(n) = 14*a(n-1) - a(n-2); a(0) = 0, a(1) = 8.
 * https://oeis.org/A067900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2), a(0) = 0, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache  = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise cache with first two terms */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;  /* allocation failed, return 0 */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* if requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0;  /* allocation failed, return 0 */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 14 * cache[i-1] - cache[i-2];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
