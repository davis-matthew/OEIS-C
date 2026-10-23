/*
 * A170261  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*Σ_{k=1}^{40} a(n-k) - 55*a(n-41) + p(n) where p(n)=1
 *            for n=0 or 41, p(n)=2 for 1≤n≤40, and p(n)=0 otherwise (this
 *            recurrence follows from the rational generating function)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache of computed values – allows repeated calls in any order. */
    static seq_t *cache   = NULL;   /* array of a(0) … a(k) */
    static idx_t cache_len = 0;     /* number of entries currently stored */

    /* Grow the cache if necessary and compute missing terms. */
    if (cache_len <= n)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0; /* memory allocation failure */

        /* Copy already cached values. */
        if (cache)
        {
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* Compute the missing terms, starting from cache_len. */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* Summation of a(i-1) … a(i-40) with coefficient 10. */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 40 && i >= k; ++k)
                sum += (seq_t)10 * new_cache[i - k];

            /* Subtract 55*a(i-41) if it exists. */
            if (i >= 41)
                sum -= (seq_t)55 * new_cache[i - 41];

            /* Add the polynomial coefficient p(i). */
            seq_t p;
            if (i == 0 || i == 41)
                p = (seq_t)1;
            else if (i <= 40)
                p = (seq_t)2;
            else
                p = (seq_t)0;

            new_cache[i] = sum + p;
        }

        cache_len = new_len;
        cache = new_cache;
    }

    return cache[n];
}
