/*
 * A168721  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A168721: a(n) = coefficient of t^n in (t^17+2t^16+…+2t+1)/(903t^17−42t^16−…−42t+1).  
   Recurrence for n≥17: a(n)=42·a(n-1)+⋯+42·a(n-16)−903·a(n-17).  
   Initial values a(0) … a(16) are obtained from the numerator:  
   a(0)=1, a(k)=2 for 1≤k≤16, a(17)=1.  This routine memoises the results
   and works for arbitrary n. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t val;
            /* coefficient of t^k in the numerator */
            if (k == 0)            val = (seq_t)1;
            else if (k <= 16)      val = (seq_t)2;
            else if (k == 17)      val = (seq_t)1;
            else                   val = (seq_t)0;

            /* add 42·a(k-j) for j=1..16 (or up to k) */
            idx_t jmax = (k < 16) ? k : 16;
            for (idx_t j = 1; j <= jmax; ++j)
                val += (seq_t)42 * cache[k - j];

            /* subtract 903·a(k-17) if applicable */
            if (k >= 17)
                val -= (seq_t)903 * cache[k - 17];

            cache[k] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
