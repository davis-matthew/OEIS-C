/*
 * A038723  a(n) = 6*a(n-1) - a(n-2), n >= 2, a(0)=1, a(1)=4.
 * https://oeis.org/A038723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;   /* capacity of the array   */
    static idx_t cache_len = 0;   /* number of computed terms */

    /* if requested term already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache if necessary */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) /* allocation failed: abort */
            abort();
        cache = new_cache;
        cache_cap = needed;
    }

    /* initialise base cases if this is the first call */
    if (cache_len == 0) {
        cache[0] = 1;
        if (needed > 1)
            cache[1] = 4;
        cache_len = (needed > 1) ? 2 : 1;
    } else if (cache_len == 1) {
        /* only a(0) present, set a(1) if needed */
        cache[1] = 4;
        cache_len = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = cache[i-1] * 6 - cache[i-2];

    cache_len = needed;
    return cache[n];
}
