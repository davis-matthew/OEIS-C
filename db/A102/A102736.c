/*
 * A102736  Number of permutations of n elements without cycles whose length
 *          is a multiple of 3.
 * https://oeis.org/A102736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of permutations of n elements with no cycle of
 *            length a multiple of 3 Recurrence: a(0)=1; for n≥1 a(n) =
 *            Σ_{k=1..n, k%3≠0} (n-1)!/(n-k)! * a(n-k) The factor
 *            (n-1)!/(n-k)! = Π_{t=n-k+1}^{n-1} t
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure cache large enough */
    if (n >= a_cache_size)
    {
        idx_t newSize = n + 1;
        a_cache = (seq_t *)realloc(a_cache, newSize * sizeof(seq_t));
        if (a_cache == NULL)
            return (seq_t)0; /* out of memory, return 0 as error */
        /* Compute new entries */
        for (idx_t i = a_cache_size; i <= n; ++i)
        {
            if (i == 0)
            {
                a_cache[0] = (seq_t)1;
                continue;
            }
            seq_t prod = (seq_t)1;
            seq_t sum  = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k)
            {
                if (k % 3 != 0)
                    sum += prod * a_cache[i - k];
                if (k < i)
                    prod *= (seq_t)(i - k);
            }
            a_cache[i] = sum;
        }
        a_cache_size = newSize;
    }
    return a_cache[n];
}
