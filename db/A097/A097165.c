/*
 * A097165  Expansion of (1-3x)/((1-x)(1-4x)(1-5x)).
 * https://oeis.org/A097165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=10*a(n-1)-29*a(n-2)+20*a(n-3);  a(0)=1, a(1)=7, a(2)=41 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size)
    {
        /* Reallocate cache to store values up to n */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t val;
            if (i == 0)
                val = 1;
            else if (i == 1)
                val = 7;
            else if (i == 2)
                val = 41;
            else
            {
                /* Use the recurrence relation */
                val = 10 * cache[i - 1] - 29 * cache[i - 2] + 20 * cache[i - 3];
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
