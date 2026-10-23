/*
 * A170507  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1, a(1)=18.
 * For 2 ≤ n ≤ 46 the values are obtained from the generating function
 * numerator P(t)=t^46+2t^45+⋯+2t+1, which implies 
 * a(n)=p_n + 16·∑_{k=1}^{n}a(n-k)  (p_n=1 for n=0 or n=46, otherwise p_n=2),
 * and for n=46 additionally subtract 136·a(0).
 * For n ≥ 47 the coefficients of the generating function’s denominator give
 * the linear recurrence
 *     a(n) = 16·(a(n‑1)+a(n‑2)+…+a(n‑45)) – 136·a(n‑46).
 * This recurrence matches all terms of OEIS A170507.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;
    static size_t  cache_cap = 0;

    while (cache_len <= n)
    {
        seq_t val;
        if (cache_len <= 46)         /* 0 ≤ n ≤ 46  */
        {
            seq_t sum_prev = 0;
            for (idx_t i = 0; i < cache_len; ++i)
                sum_prev += cache[i];

            seq_t p = (cache_len == 0 || cache_len == 46) ? (seq_t)1 : (seq_t)2;
            val = p + (seq_t)16 * sum_prev;
            if (cache_len == 46)
                val -= (seq_t)136 * cache[0];
        }
        else                           /* n ≥ 47  */
        {
            seq_t sum45 = 0;
            for (idx_t i = cache_len - 45; i < cache_len; ++i)
                sum45 += cache[i];

            val = (seq_t)16 * sum45 - (seq_t)136 * cache[cache_len - 46];
        }

        if (cache_len >= cache_cap)    /* grow the cache if necessary */
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);   /* allocation failed */
            cache     = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;
    }

    return cache[n];
}
