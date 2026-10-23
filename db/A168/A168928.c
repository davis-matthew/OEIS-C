/*
 * A168928  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 5*a(n-21) for n >= 22, a(0) = 1, and for 1 <=
 *            n <= 21, a(n) = 11*10^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: pow10 is provided by ../utils/pow10.h. */
#include <stdlib.h>
#include <string.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed values. */
    static seq_t *cache = NULL;      /* array of computed a(i). */
    static idx_t cache_size = 0;     /* next free index in cache (size). */

    /* Grow the cache to cover index n. */
    if (n >= cache_size)
    {
        /* Allocate a larger array. */
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache && cache_size)
            memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));

        /* Compute new values. */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0)
                new_cache[0] = (seq_t)1;                         /* a(0) = 1 */
            else if (i <= 21)
                new_cache[i] = (seq_t)11 * pow10((idx_t)(i - 1));/* a(i) = 11*10^(i-1) */
            else
            {
                /* a(i) = 10*a(i-1) - 5*a(i-21) */
                new_cache[i] = (seq_t)10 * new_cache[i - 1]
                               - (seq_t)5 * new_cache[i - 21];
            }
        }

        /* Replace the old cache. */
        if (cache)
            free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
