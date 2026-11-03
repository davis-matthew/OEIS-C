/*
 * A102902  a(n) = 9*a(n-1) - 16*a(n-2), with a(0) = 1, a(1) = 9.
 * https://oeis.org/A102902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - 16*a(n-2); a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 1172 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Computes A102902.  Values are stored in a static cache so that multiple calls
 * with arbitrary n are fast.  The cache grows geometrically to keep the
 * amortised cost of insertions O(1). */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;      /* array of cached terms */
    static idx_t  cache_len = 0;     /* number of terms currently computed */
    static idx_t  cache_cap = 0;     /* allocated capacity of the array */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap = 2;                    /* at least two terms for base cases */
        cache     = mem_new(seq_t, cache_cap);
        if (cache == NULL)
            return 0;                    /* fallback: cannot allocate */
        cache[0] = (seq_t)1;              /* a(0) */
        cache[1] = (seq_t)9;              /* a(1) */
        cache_len = 2;
    }

    /* If n is already computed, return immediately */
    if (n < cache_len)
        return cache[n];

    /* ensure sufficient capacity */
    if (n + 1 > cache_cap)
    {
        while (cache_cap < n + 1)
            cache_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
        if (tmp == NULL)
            return 0;                    /* fallback: cannot reallocate */
        cache = tmp;
    }

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* a(i) = 9*a(i-1) - 16*a(i-2) */
        seq_t val = (seq_t)9 * cache[i - 1] - (seq_t)16 * cache[i - 2];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
