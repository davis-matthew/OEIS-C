/*
 * A180036  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1 - 2*x)/(1 - 5*x - 3*x^2).
 * https://oeis.org/A180036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure enough capacity for n+1 entries */
    size_t need = (size_t)n + 1;
    if (need > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < need) new_cap <<= 1;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;              /* allocation failed */

        if (cache)
        {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base terms if they are not already set */
    if (cache_len < 2)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    for (size_t i = cache_len; i < need; ++i)
    {
        cache[i] = cache[i - 1] * (seq_t)5 + cache[i - 2] * (seq_t)3;
    }
    cache_len = need;

    return cache[n];
}
