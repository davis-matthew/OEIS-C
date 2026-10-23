/*
 * A033191  Binomial transform of [ 1, 0, 1, 1, 3, 6, 15, 36, 91, 231, 595,
 *          ... ], which is essentially binomial(Fibonacci(k) + 1, 2).
 * https://oeis.org/A033191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14,
 * a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4)  for n >= 5 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries stored in cache */

    /* enlarge cache if necessary */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;                /* need indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                    /* failed to grow the cache */
        cache = tmp;

        /* initialise first five terms if they are not yet present */
        if (cache_len < 5)
        {
            static const seq_t init[5] = {1, 1, 2, 5, 14};
            idx_t i;
            for (i = cache_len; i < 5 && i < new_len; ++i)
                cache[i] = init[i];
        }

        /* compute terms by the recurrence, starting at index 5 */
        idx_t i, start = (cache_len > 5) ? cache_len : 5;
        for (i = start; i < new_len; ++i)
        {
            /*  a(i) = 8*a(i-1) - 21*a(i-2) + 20*a(i-3) - 5*a(i-4) */
            seq_t t1 = 8 * cache[i-1];
            seq_t t2 = 21 * cache[i-2];
            seq_t t3 = 20 * cache[i-3];
            seq_t t4 = 5  * cache[i-4];
            cache[i] = t1 - t2 + t3 - t4;
        }

        cache_len = new_len;
    }

    return cache[n];
}
