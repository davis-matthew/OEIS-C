/*
 * A165548  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=32, a(2)=992, a(3)=30752, a(4)=953312,
 *            a(5)=29552672, a(6)=916132832, a(7)=28400117792,
 *            a(8)=880403651552, a(9)=27292513197616, for n≥0:
 *            a(n)=30*(a(n-1)+...+a(n-8))+p(n)-465*a(n-9), where p(n)=1 if n=0
 *            or n=9, p(n)=2 if 1≤n≤8, and p(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* coefficients of numerator P(t) = t^9 + 2t^8 + … + 2t + 1 */
    static const seq_t p_coeff[10] = {
        (seq_t)1ULL, (seq_t)2ULL, (seq_t)2ULL, (seq_t)2ULL,
        (seq_t)2ULL, (seq_t)2ULL, (seq_t)2ULL, (seq_t)2ULL,
        (seq_t)2ULL, (seq_t)1ULL
    };

    /* grow cache if necessary */
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();          /* allocation failed */
        cache = tmp;

        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            /* sum of last eight terms a(i‑1)…a(i‑8) */
            seq_t sum8 = 0;
            for (idx_t k = 1; k <= 8 && i >= k; ++k)
                sum8 += cache[i - k];

            /* base coefficient p(i) */
            seq_t pi = (i < 10) ? p_coeff[i] : (seq_t)0;

            /* a(i) = 30·sum8 + p(i) – 465·a(i‑9)  (if i≥9) */
            seq_t ai = (seq_t)30 * sum8 + pi;
            if (i >= 9)
                ai -= (seq_t)465 * cache[i - 9];

            cache[i] = ai;
        }
        cache_size = new_size;
    }
    return cache[n];
}
