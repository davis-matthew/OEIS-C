/*
 * A098152  a(n) = a(n-1)^2 + n, with a(0)=0.
 * https://oeis.org/A098152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + n, with a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i <= cache_max. */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* First call: allocate cache up to n and fill values. */
    if (cache == NULL) {
        size_t sz = (size_t)n + 1;
        if (sz == 0) sz = 1;                /* safe when n == 0 */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure: return 0 */
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * cache[i - 1] + (seq_t)i;
        }
        cache_max = n;
        return cache[n];
    }

    /* If requested index is within the already computed range. */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Grow the cache to accommodate the new index. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* If realloc fails, keep existing cache intact and return last known value. */
        return cache[cache_max];
    }
    cache = tmp;

    /* Compute new values from the previous maximum up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * cache[i - 1] + (seq_t)i;
    }
    cache_max = n;
    return cache[n];
}
