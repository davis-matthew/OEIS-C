/*
 * A170408  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(0) = 1,
 *           a(n) = 1 + 13·∑_{k=1}^{43} a(n-k) – 91·a(n-44)   for n = 44
 *           a(n) = 2 + 13·∑_{k=1}^{min(n,43)} a(n-k)          for 1 ≤ n ≤ 43
 *           a(n) = 13·∑_{k=1}^{43} a(n-k) – 91·a(n-44)        for n ≥ 45
 *
 * The first 44 terms are obtained from the numerator of the
 * generating function; thereafter the linear recurrence of order 44
 * (derived from the denominator) is used.  The function is fully
 * memoised and supports calls in any order.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* number of terms stored: cache[0..cached-1] */

    /* Grow the cache if required */
    if (n >= cached)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                /* allocation failure – unlikely in tests */
            return (seq_t)0;
        cache = tmp;

        /* Compute missing terms up to n */
        for (idx_t i = cached; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[i] = (seq_t)1;                   /* a(0) */
            }
            else if (i <= 44)                       /* 1 ≤ i ≤ 44 */
            {
                seq_t coeff = (i == 44) ? (seq_t)1 : (seq_t)2;
                seq_t sum   = (seq_t)0;
                for (idx_t k = 1; k <= i && k <= 43; ++k)
                    sum += (seq_t)13 * cache[i - k];
                if (i == 44)                     /* subtract 91*a(0) */
                    sum -= (seq_t)91 * cache[0];
                cache[i] = coeff + sum;
            }
            else                                   /* i ≥ 45 : recurrence */
            {
                seq_t sum = (seq_t)0;
                for (idx_t k = 1; k <= 43; ++k)
                    sum += (seq_t)13 * cache[i - k];
                sum -= (seq_t)91 * cache[i - 44];
                cache[i] = sum;
            }
        }
        cached = n + 1;
    }

    return cache[n];
}
