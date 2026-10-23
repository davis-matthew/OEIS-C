/*
 * A169534  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based implementation of OEIS A169534
   a(0) = 1
   For n >= 0:
      coeff(n) = 1  if n==0 or n==34
               = 2  if 1 <= n <= 33
               = 0  otherwise
      a(n) = coeff(n)
             + 39 * (a(n-1) + a(n-2) + ... + a(n-33))   (terms with negative index are omitted)
             - (n >= 34 ? 780 * a(n-34) : 0)             (the subtraction term is omitted for n < 34)
*/

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed terms (cache_len == current capacity) */

    if ((size_t)n + 1 > cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
        {   /* allocation failed; return zero as a safe fallback */
            if (cache) free(cache);
            return 0;
        }
        cache = new_cache;

        /* compute missing terms from cache_len to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i)
        {
            seq_t coeff;
            if (i == 0 || i == 34) coeff = 1;
            else if (i <= 33)       coeff = 2;
            else                   coeff = 0;

            seq_t sum = 0;

            /* add 39 * a(i-k) for k = 1 .. 33, if i >= k */
            for (size_t k = 1; k <= 33; ++k)
            {
                if (i >= k)
                    sum = sum + 39 * cache[i - k];
            }

            /* subtract 780 * a(i-34) when i >= 34 */
            if (i >= 34)
                sum = sum - 780 * cache[i - 34];

            cache[i] = coeff + sum;
        }

        cache_len = new_len;
    }

    return cache[n];
}
