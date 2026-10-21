/*
 * A098589  a(n) = 3*a(n-1) + 2*a(n-3), with a(0)=1, a(1)=3.
 * https://oeis.org/A098589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-3), with a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize cache with the three base values. */
    if (cache_len < 3) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
    }

    /* If the requested index is already computed, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to at least n+1 entries. */
    idx_t new_len = n + 1;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) /* allocation failed – program should abort */ return 0;
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t v1 = cache[i - 1];
            v1 *= (seq_t)3;
            seq_t v2 = cache[i - 3];
            v2 *= (seq_t)2;
            cache[i] = v1 + v2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
