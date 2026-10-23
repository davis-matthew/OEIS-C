/*
 * A170420  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   for n≥1
      a(n) = 25 * Σ_{i=1}^{min(43,n)} a(n-i) + b(n) - (n≥44 ? 325 * a(n-44) : 0),
   where b(0)=1, b(44)=1, b(n)=2 for 1≤n≤43, otherwise b(n)=0. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored */

    if (n >= cache_len)
    {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        for (idx_t k = old_len; k <= n; ++k)
        {
            seq_t sumPrev = 0;
            for (idx_t i = 1; i <= 43 && k >= i; ++i)
                sumPrev += cache[k - i];

            seq_t val = 25 * sumPrev;
            if (k >= 44)
                val -= 325 * cache[k - 44];

            seq_t b = 0;
            if (k == 0 || k == 44)
                b = 1;
            else if (k <= 43)
                b = 2;

            val += b;
            cache[k] = val;
        }
    }
    return cache[n];
}
