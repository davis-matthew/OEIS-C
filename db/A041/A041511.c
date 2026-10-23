/*
 * A041511  Denominators of continued fraction convergents to sqrt(272).
 * https://oeis.org/A041511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 66*a(n-2) - a(n-4) for n>3 a(0)=1, a(1)=2, a(2)=65,
 *            a(3)=132
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* If the desired index is beyond the current cache, enlarge it. */
    if (n + 1 > (idx_t)cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure, return zero as a default */
        cache = tmp;

        /* Fill the cache up to index n. */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)2;
            else if (i == 2)
                cache[2] = (seq_t)65;
            else if (i == 3)
                cache[3] = (seq_t)132;
            else
                cache[i] = (seq_t)66 * cache[i - 2] - cache[i - 4];
        }
        cache_size = new_size;
    }
    return cache[n];
}
