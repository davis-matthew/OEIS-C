/*
 * A166854  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) =
 *            46·a(n-1)+46·a(n-2)+…+46·a(n-11) – 1081·a(n-12) + b(n) * * where
 *            b(n) = 1 for n = 0 or 12, * 2 for 1 ≤ n ≤ 11, * 0 otherwise. *
 *            The sequence starts with a(0)=1, a(1)=48, … . * * This
 *            implementation memoises computed values in a static cache and *
 *            comp...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow the cache if necessary */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failed – return zero as sentinel */

        cache = new_cache;

        /* Compute all missing terms */
        for (idx_t i = cache_size; i < new_size; ++i)
        {
            /* Determine the inhomogeneous term b(n) */
            seq_t b;
            if (i == 0 || i == 12)
                b = (seq_t)1;
            else if (i < 12)
                b = (seq_t)2;
            else
                b = (seq_t)0;

            /* Sum of up to the last 11 terms */
            seq_t sum_prev = (seq_t)0;
            for (idx_t j = 1; j <= 11 && j <= i; ++j)
                sum_prev += cache[i - j];

            /* 46·sum_prev */
            seq_t value = (seq_t)46 * sum_prev;

            /* Subtract 1081·a(n-12) for n ≥ 12 */
            if (i >= 12)
                value -= (seq_t)1081 * cache[i - 12];

            /* Add the b(n) term */
            cache[i] = value + b;
        }

        cache_size = new_size;
    }

    return cache[n];
}
