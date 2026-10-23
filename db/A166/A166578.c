/*
 * A166578  a(n) = a(n-3) + 2^(n-4) with a(1) = 1, a(2) = 2, a(3) = 1.
 * https://oeis.org/A166578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*2 + a(n-3) - 2*a(n-4) for n > 3 initial values:
 *            a(0)=1, a(1)=2, a(2)=1, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"   /* provide ensure_cache(idx_t n, seq_t **cache, size_t *cap) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* capacity of cache array */
    static size_t len = 0;           /* number of values already computed (len = highest_computed + 1) */

    /* guarantee storage for requested index */
    ensure_cache(n, &cache, &cap);

    /* fill in missing values */
    for (idx_t i = (idx_t)len; i <= n; ++i)
    {
        if (i == 0)                 cache[i] = (seq_t)1;
        else if (i == 1)            cache[i] = (seq_t)2;
        else if (i == 2)            cache[i] = (seq_t)1;
        else if (i == 3)            cache[i] = (seq_t)2;
        else                        cache[i] = (seq_t)2 * cache[i-1] + cache[i-3] - (seq_t)2 * cache[i-4];
    }
    len = (size_t)n + 1;   /* update highest computed index */

    return cache[n];
}
