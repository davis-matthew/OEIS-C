/*
 * A158609  Expansion of (1+8*x)/(1-x-81*x^2).
 * https://oeis.org/A158609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 81*a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;  /* number of terms currently stored */

    /* Initialise the cache on first call */
    if (cache == NULL)
    {
        ensure_cache(2, &cache, &cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        computed = 2;
    }

    /* Make sure we can address cache[n] */
    if ((size_t)n + 1 > cap)
        ensure_cache((size_t)n + 1, &cache, &cap);

    /* Compute any missing terms up to n */
    if ((size_t)n >= computed)
    {
        for (size_t i = computed; i <= (size_t)n; ++i)
        {
            seq_t prod = cache[i - 2] * 81u;
            cache[i] = cache[i - 1] + prod;
        }
        computed = (size_t)n + 1;
    }

    return cache[n];
}
