/*
 * A167658  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^{14} 7*a(n-k) - 28*a(n-15) + delta(n) where
 *            delta(n) = 1 if n==0 or n==15, = 2 if 1 <= n <= 14, = 0
 *            otherwise
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the linear recurrence derived from the generating function. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Static cache of computed terms.  */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* one past the last filled index */

    /*  Grow the cache if necessary.  */
    if ((size_t)n >= cap)
    {
        size_t new_cap = (size_t)n + 1;          /* need indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failure */
        cache = tmp;

        /*  Compute missing terms using the recurrence.  */
        for (size_t i = cap; i < new_cap; ++i)
        {
            seq_t sum = 0;

            /*  Sum 7*a(i-k) for k=1..14  */
            for (size_t k = 1; k <= 14; ++k)
            {
                if (i >= k)
                {
                    sum += (seq_t)7 * cache[i - k];
                }
            }

            /*  Subtract 28*a(i-15) if i >= 15  */
            if (i >= 15)
            {
                sum -= (seq_t)28 * cache[i - 15];
            }

            /*  Add the delta term  */
            seq_t delta = 0;
            if (i == 0 || i == 15)
                delta = 1;
            else if (i > 0 && i < 15)
                delta = 2;
            /*  For i > 15 the delta is already 0.  */

            cache[i] = sum + delta;
        }

        cap = new_cap;
    }

    return cache[(size_t)n];
}
