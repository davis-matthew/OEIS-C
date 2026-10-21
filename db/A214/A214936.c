/*
 * A214936  a(0) = 1, a(n) = a(n - 1) * (length of binary representation of
 *          n).
 * https://oeis.org/A214936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(n) = a(n-1) * (length of binary representation of n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    /* Ensure cache[0]=1 is initialized */
    if (cache_len == 0) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * 1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If n already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend cache to size n+1 */
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!tmp) {
        /* allocation failure: in a real build, handle error appropriately */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute missing terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute number of bits in binary representation of i */
        idx_t tmp_i = i;
        idx_t bitlen = 0;
        while (tmp_i) {
            ++bitlen;
            tmp_i >>= 1;
        }
        /* For i > 0, bitlen >= 1. For safety, when i == 0 (already done), skip. */
        if (i > 0) {
            seq_t mult = (seq_t)bitlen;
            cache[i] = cache[i - 1] * mult;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
