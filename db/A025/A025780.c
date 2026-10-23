/*
 * A025780  Expansion of 1/((1-x)(1-x^5)(1-x^11)).
 * https://oeis.org/A025780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 5, and 11
 *            (coefficient of x^n in 1/((1-x)(1-x^5)(1-x^11)))
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* If the cache is empty or too small, recompute up to n */
    if (cache == NULL || n > cache_max)
    {
        idx_t new_max = n;
        size_t size = (size_t)(new_max + 1);            /* number of entries */
        seq_t *new_cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (new_cache == NULL)
            exit(1);                                  /* allocation failure */

        /* initialize to zero and set base case */
        for (idx_t i = 0; i <= new_max; ++i)
            new_cache[i] = 0;
        new_cache[0] = 1;

        /* update for part size 1 */
        for (idx_t i = 1; i <= new_max; ++i)
            new_cache[i] += new_cache[i - 1];

        /* update for part size 5 */
        if (new_max >= 5)
        {
            for (idx_t i = 5; i <= new_max; ++i)
                new_cache[i] += new_cache[i - 5];
        }

        /* update for part size 11 */
        if (new_max >= 11)
        {
            for (idx_t i = 11; i <= new_max; ++i)
                new_cache[i] += new_cache[i - 11];
        }

        /* replace old cache */
        if (cache != NULL)
            free(cache);
        cache = new_cache;
        cache_max = new_max;
    }

    return cache[n];
}
