/*
 * A169044  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For n≥1 the coefficients satisfy the linear recurrence
 *            derived from the rational generating function A(t) = (t^24 + 2
 *            t^23 + ... + 2 t + 1) / (435 t^24 - 29 t^23 - ... - 29 t + 1).
 *            That leads to a(n) - 29 ∑_{k=1}^{23} a(n-k) + 435 a(n-24) =
 *            b(n), where b(0)=1, b(n)=2 for 1≤n≤23, b(24)=1,...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len)
    {
        size_t new_len = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t b;
            if (i == 0)
                b = (seq_t)1;
            else if (i <= 24)
                b = (i == 24 ? (seq_t)1 : (seq_t)2);
            else
                b = (seq_t)0;

            seq_t sum23 = (seq_t)0;
            for (idx_t k = 1; k <= 23 && k <= i; ++k)
                sum23 += ((seq_t)29) * cache[i - k];

            seq_t sub = (seq_t)0;
            if (i >= 24)
                sub = ((seq_t)435) * cache[i - 24];

            cache[i] = b + sum23 - sub;
        }
        cache_len = new_len;
    }
    return cache[n];
}
