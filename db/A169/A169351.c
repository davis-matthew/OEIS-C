/*
 * A169351  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1)+48*a(n-2)+...+48*a(n-29)-1176*a(n-30)+p(n)
 *            where p(0)=1, p(1..29)=2, p(30)=1, p(n)=0 for n>30
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
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
    static size_t cache_len = 0;  /* first unused index */

    /* extend cache if necessary */
    if ((size_t)n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;

        if (cache == NULL)
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));

        if (cache == NULL)
            return 0;           /* allocation failure – unlikely */

        /* compute missing terms */
        for (size_t i = cache_len; i < new_len; ++i)
        {
            seq_t sum = 0;

            /*   sum_{k=1}^{29} 48*a(i-k)   */
            for (size_t k = 1; k <= 29 && k <= i; ++k)
                sum += (seq_t)48 * cache[i - k];

            /*   numerator contribution p_i   */
            if (i <= 30)
                sum += (i == 0 || i == 30) ? (seq_t)1 : (seq_t)2;

            /*   subtract 1176*a(i-30) for i ≥ 30   */
            if (i >= 30)
                sum -= (seq_t)1176 * cache[i - 30];

            cache[i] = sum;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
