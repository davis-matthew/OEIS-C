/*
 * A041287  Denominators of continued fraction convergents to sqrt(156).
 * https://oeis.org/A041287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 50*a(n-2) - a(n-4) Initial values: a(0)=1, a(1)=2,
 *            a(2)=49, a(3)=100
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values, computed on first use. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Number of entries needed (0..n inclusive). */
    size_t required = (size_t)n + 1;

    if (cache == NULL || cache_size < required)
    {
        size_t new_size = required;
        seq_t *tmp;

        if (cache == NULL)
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        if (!tmp)
            return 0;                    /* allocation failed */

        cache = tmp;

        /* Compute all new entries, starting from the old cache_size. */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i == 0)
                cache[i] = 1;
            else if (i == 1)
                cache[i] = 2;
            else if (i == 2)
                cache[i] = 49;
            else if (i == 3)
                cache[i] = 100;
            else
                cache[i] = ((seq_t)50 * cache[i - 2]) - cache[i - 4];
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
