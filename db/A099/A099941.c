/*
 * A099941  a(0) = 4; for n>0, a(n) = a(n-1)^2 - 10.
 * https://oeis.org/A099941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 4; a(n) = a(n-1)^2 – 10 for n > 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
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
    static idx_t cache_cap = 0;

    /* increase capacity while necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise base value if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    /* compute terms up to n, extending the cache as needed */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] * cache[i-1] - (seq_t)10;
    }
    cache_len = n + 1;

    return cache[n];
}
