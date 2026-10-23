/*
 * A163802  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1, a(1) = 46, a(2) = 2070, a(3) = 93150, a(4) = 4191750
 *            * a(5) = 44*a(4)+44*a(3)+44*a(2)+44*a(1)-990*a(0)+1 * a(n) =
 *            44*a(n-1)+44*a(n-2)+44*a(n-3)+44*a(n-4)-990*a(n-5) for n >= 6 *
 *            * The sequence satisfies the linear recurrence derived from the
 *            rational generating * function (t^5 + 2*t^4...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
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
    static size_t cache_len = 0;   /* number of computed entries */

    /* initialise cache with the first 5 terms */
    if (cache == NULL)
    {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)46;
        cache[2] = (seq_t)2070;
        cache[3] = (seq_t)93150;
        cache[4] = (seq_t)4191750;
    }

    /* if n is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow cache up to n, computing missing terms */
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_len)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* allocation failure */
        cache = tmp;

        /* compute terms from cache_len up to n */
        for (size_t i = cache_len; i <= n; ++i)
        {
            seq_t s = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
            seq_t term;
            if (i == 5)  /* special handling for a(5) */
            {
                term = (seq_t)44 * s - (seq_t)990 * cache[0] + (seq_t)1;
            }
            else
            {
                term = (seq_t)44 * s - (seq_t)990 * cache[i-5];
            }
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
