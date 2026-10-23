/*
 * A168880  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 21: a(n) = 9 * ( a(n-1) + a(n-2) + … + a(n-20) ) – 44 *
 *            a(n-21) Initial values: a(0) = 1 a(k) = 11 * 10^(k-1) for 1 ≤ k
 *            ≤ 20
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cache[0 .. cache_len-1] holds computed values */
    static idx_t cache_len = 0;        /* current cache length (number of stored terms) */
    idx_t i, k;

    /* Grow the cache to hold term n if necessary */
    if (n + 1 > cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp && new_len > 0)        /* allocation failed */
            return (seq_t)0;
        cache = tmp;

        /* Initialise a(0) if the cache was previously empty */
        if (cache_len == 0)
            cache[0] = (seq_t)1;

        /* Compute simple initial terms up to k = 20 if they are not yet stored */
        if (cache_len <= 20)
        {
            seq_t pow10 = (seq_t)1;     /* 10^(k-1) for the next term */
            for (i = 1; i <= 20 && i < new_len; ++i)
            {
                cache[i] = (seq_t)11 * pow10;
                pow10 *= (seq_t)10;
            }
        }

        /* Compute remaining terms using the recurrence */
        idx_t start = 21;               /* first index that needs the recurrence */
        if (cache_len > start)          /* if we already have values beyond 21 */
            start = cache_len;
        if (start < 21)                  /* ensure start is at least 21 */
            start = 21;

        for (i = start; i < new_len; ++i)
        {
            seq_t sum = 0;
            for (k = 1; k <= 20; ++k)
                sum += cache[i - k];
            cache[i] = (seq_t)9 * sum - (seq_t)44 * cache[i - 21];
        }

        cache_len = new_len;
    }

    return cache[n];
}
