/*
 * A017885  Expansion of
 *          1/(1-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17-x^18).
 * https://oeis.org/A017885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 75, u32 n <= 136, u64 n <= 259,
 *            u128 n <= 506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = a(n-9) + a(n-10) + a(n-11) + a(n-12) + a(n-13) +
 *          a(n-14) + a(n-15) + a(n-16) + a(n-17) + a(n-18)   for n > 17
 *   a(0) = 1
 *   a(1) = a(2) = … = a(8) = 0
 *   a(9) = a(10) = … = a(17) = 1
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* dynamic cache storing computed values */
    static idx_t  cache_len = 0;    /* length of the cache (number of elements) */

    /* Grow the cache lazily if more values are needed */
    if (n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;                 /* new required length */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                            /* allocation failure */

        cache = tmp;

        /* Compute any new entries that have not been set yet */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == (idx_t)0)
                cache[i] = (seq_t)1;
            else if (i < (idx_t)9)
                cache[i] = (seq_t)0;
            else if (i <= (idx_t)17)
                cache[i] = (seq_t)1;
            else
            {
                seq_t sum = (seq_t)0;
                /* add the last ten values from positions i-9 … i-18 */
                for (int k = 9; k <= 18; ++k)
                    sum += cache[i - k];
                cache[i] = sum;
            }
        }
        cache_len = new_len;        /* record the enlarged size */
    }

    return cache[n];
}
