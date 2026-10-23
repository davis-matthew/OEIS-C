/*
 * A167026  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the given generating function:
   Let N(n) = coefficients of the numerator polynomial,
   N(0)=1, N(1..12)=2, N(13)=1, N(n)=0 for n>13.
   For n >= 0:
       a(n) = sum_{k=1}^{min(12,n)} 14*a(n-k) + N(n)
       if n >= 13 subtract 105*a(n-13)
   This reproduces the first terms listed in A167026.
*/
#include <stdlib.h>

static const unsigned char Ncoeff[14] = {
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1
};

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    /* Expand cache up to target if necessary */
    if (target >= cache_len)
    {
        size_t new_len = target + 1;
        /* Allocate or resize the cache array */
        if (cache_len == 0)
        {
            cache = (seq_t*)calloc(new_len, sizeof(seq_t));
        }
        else
        {
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        }
        /* Compute new terms sequentially */
        for (size_t i = cache_len; i <= target; ++i)
        {
            seq_t sum = 0;
            /* Add the numerator coefficient if within range */
            if (i < 14)
                sum += Ncoeff[i];

            /* Sum 14 * a(i-k) for k = 1 .. min(12,i) */
            size_t maxk = (i < 12) ? i : 12;
            for (size_t k = 1; k <= maxk; ++k)
            {
                sum += (seq_t)14 * cache[i - k];
            }

            /* Subtract 105 * a(i-13) if i >= 13 */
            if (i >= 13)
                sum -= (seq_t)105 * cache[i - 13];

            cache[i] = sum;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
