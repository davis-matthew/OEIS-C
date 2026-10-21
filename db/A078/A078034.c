/*
 * A078034  Expansion of (1-x)/(1+2*x^2-2*x^3).
 * https://oeis.org/A078034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-1, a(2)=-2, a(n)= -2*a(n-2)+2*a(n-3) for n>=3
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms */
    static idx_t  cache_len = 0;         /* next free index (size of cache) */

    if (n >= cache_len)                 /* grow cache up to n (inclusive) */
    {
        idx_t new_len = n + 1;           /* we need indices 0..n       */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;

        /* compute new values */
        for (idx_t k = cache_len; k <= n; ++k)
        {
            if (k == 0)
                cache[k] = 1;
            else if (k == 1)
                cache[k] = -1;
            else if (k == 2)
                cache[k] = -2;
            else
                cache[k] = 2 * (cache[k - 3] - cache[k - 2]); /* a(k) = -2*a(k-2)+2*a(k-3) */
        }
        cache_len = new_len;
    }

    return cache[n];
}
