/*
 * A175378  G.f. x^4*(2*x^2-1)/( (x^2-1)*(x^2+x-1)*(2*x^3-2*x^2+2*x-1) ).
 * https://oeis.org/A175378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 3*a(n-4) - 4*a(n-5) +
 *            2*a(n-7) Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1,
 *            a(5)=3, a(6)=5, a(7)=8
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* extend cache if needed */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t val;
            if (i == 0)        val = 0;
            else if (i == 1)   val = 0;
            else if (i == 2)   val = 0;
            else if (i == 3)   val = 0;
            else if (i == 4)   val = 1;
            else if (i == 5)   val = 3;
            else if (i == 6)   val = 5;
            else if (i == 7)   val = 8;
            else
            {
                /* split positives and negatives to avoid unsigned underflow */
                seq_t pos = 3 * cache[i-1] + 3 * cache[i-4] + 2 * cache[i-7];
                seq_t neg = 2 * cache[i-2] + cache[i-3] + 4 * cache[i-5];
                val = pos - neg; /* pos >= neg always for this sequence */
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
