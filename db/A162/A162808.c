/*
 * A162808  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=22, a(2)=462, a(n)=20*a(n-1)+20*a(n-2)-210*a(n-3)+
 *            (n==3?1:0) for n≥3 (derived from the rational generating
 *            function)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure the cache can hold the requested index */
    if ((size_t)n + 1 > cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache = tmp;

        /* Initialise missing entries */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i == 0u)
                cache[i] = (seq_t)1;
            else if (i == 1u)
                cache[i] = (seq_t)22;
            else if (i == 2u)
                cache[i] = (seq_t)462;
            else
            {
                seq_t term = 20 * cache[i-1] + 20 * cache[i-2] - 210 * cache[i-3];
                if (i == 3u)            /* add the N₃ term = 1 */
                    term += (seq_t)1;
                cache[i] = term;
            }
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
