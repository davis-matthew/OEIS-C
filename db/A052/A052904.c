/*
 * A052904  Expansion of (1-x)/(1-2x-4x^2+4x^3).
 * https://oeis.org/A052904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 4*a(n-3), with * a(0) = 1, a(1) =
 *            1, a(2) = 6 . Recurrence holds for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed values */
    static idx_t cache_len = 0;   /* number of entries in cache */

    /* initialise cache with the first three terms */
    if (cache == NULL)
    {
        cache_len = 3;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* out‑of‑memory, 0 is harmless */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
    }

    /* return if already computed */
    if (n < cache_len)
        return cache[(size_t)n];

    /* grow the cache to size n+1 */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;                  /* out‑of‑memory */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t term = 2 * cache[(size_t)(i - 1)]
                   + 4 * cache[(size_t)(i - 2)]
                   - 4 * cache[(size_t)(i - 3)];
        cache[(size_t)i] = term;
    }

    cache_len = (idx_t)new_len;
    return cache[(size_t)n];
}
