/*
 * A063111  Dimension of the space of weight 2n cusp forms for Gamma_0( 43 ).
 * https://oeis.org/A063111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 5, with a(1) = 3, a(2) =
 *            10, a(3) = 18, a(4) = 24, a(5) = 32, a(6) = 40
 * Range:     u8 n <= 35, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* behaviour below the offset is irrelevant */

    /* initialise the cache with the first six values */
    if (cache == NULL)
    {
        cache_size = 6;                       /* maximum index stored so far */
        cache = (seq_t *)calloc((size_t)cache_size + 1, sizeof(seq_t));
        if (!cache) abort();                  /* out of memory */
        cache[1] = 3;
        cache[2] = 10;
        cache[3] = 18;
        cache[4] = 24;
        cache[5] = 32;
        cache[6] = 40;
    }

    if (n > cache_size)
    {
        /* extend the cache up to the requested index */
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) abort();                    /* out of memory */
        cache = tmp;
        /* compute the missing terms using the recurrence */
        for (size_t i = (size_t)cache_size + 1; i <= new_size; ++i)
        {
            cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
