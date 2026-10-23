/*
 * A169683  The canonical skew-binary numbers.
 * https://oeis.org/A169683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0; for n>=1, a(2^k-1+i) = a(i) + 10^(k-1) for 0 <= i <=
 *            2^k-1
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache[0] = (seq_t)0;   /* a(0) */
    }

    /* return cached value if available */
    if (n < cache_size && cache[n] != (seq_t)0)
        return cache[n];

    /* handle base case */
    if (n == 0) {
        cache[0] = (seq_t)0;
        return cache[0];
    }

    /* compute k such that 2^k <= n+1 < 2^(k+1) */
    idx_t k = 0;
    idx_t v = n + 1;
    while (v >>= 1)
        ++k;

    /* i = n - (2^k - 1) */
    idx_t i = n - (((idx_t)1 << k) - (idx_t)1);

    /* recursive computation of a(i) */
    seq_t val = a(i);

    /* compute 10^(k-1) */
    seq_t pow10 = (seq_t)1;
    for (idx_t j = 0; j < k - 1; ++j)
        pow10 *= (seq_t)10;

    val += pow10;

    /* resize cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return value without caching */
            return val;
        }
        /* initialise new entries to 0 */
        for (size_t idx = cache_size; idx < new_size; ++idx)
            new_cache[idx] = (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    cache[n] = val;
    return val;
}
