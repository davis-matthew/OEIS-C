/*
 * A170470  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 0 < n < 45 : a(n) = 27 * Σ_{i=0}^{n-1} a(i) + 2.
 *            For n ≥ 45 : a(n) = 27 * Σ_{i=n-44}^{n-1} a(i) - 378 * a(n-45) +
 *            (n==45?1:0). This is the recurrence derived from the rational
 *            generating function (t^45 + 2 t^44 + … + 2 t + 1) / (378 t^45 –
 *            27 t^44 – … – 27 t + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of already computed terms          */
    static idx_t  cache_len = 0;  /* number of entries stored in cache        */

    /* enlarge cache if needed                                        */
    if (n + 1 > cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();                 /* out of memory      */
        cache = tmp;
        /* ----------------------------------------------------------------- */
        /* compute terms from current cache length up to n inclusive          */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)                           /* a(0) = 1                   */
            {
                cache[i] = (seq_t)1;
                continue;
            }

            /* compute sum of the required previous terms                     */
            seq_t sum = 0;
            if (i < 45)                            /* sum_{k=0}^{i-1} a(k)         */
            {
                for (idx_t j = 0; j < i; ++j)
                    sum += cache[j];
                /* N_i = 2 for i = 1..44, already accounted for i==0 above   */
                cache[i] = (seq_t)27 * sum + (seq_t)2;
            }
            else                                   /* i ≥ 45                        */
            {
                for (idx_t j = i - 44; j < i; ++j) /* sum_{k=i-44}^{i-1} a(k)      */
                    sum += cache[j];
                seq_t val = (seq_t)27 * sum
                            - (seq_t)378 * cache[i - 45];
                if (i == 45)                      /* N_45 = 1                      */
                    val += (seq_t)1;
                cache[i] = val;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
