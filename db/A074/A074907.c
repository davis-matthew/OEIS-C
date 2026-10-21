/*
 * A074907  Aliquot sequence starting at 570.
 * https://oeis.org/A074907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=570, a(n+1)=sigma(a(n))-a(n)
 * Range:     u8 none, u16 n <= 16, u32 n <= 105, u64 n <= 173, u128 n <= 173,
 *            big any n
 * Helpers:   ensure_cache, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure we have space for up to n */
    if ((size_t)n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* First term */
    if (cache_len == 0)
    {
        cache[0] = 570;
        cache_len = 1;
    }

    /* Compute missing terms */
    size_t nidx = (size_t)n;
    for (size_t i = cache_len; i <= nidx; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t sigma = sigma_of((idx_t)prev);
        cache[i] = sigma - prev;
    }
    cache_len = nidx + 1;

    return cache[nidx];
}
