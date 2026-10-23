/*
 * A169172  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the rational generating function P(t) = t^27 +
 *            2 t^26 + … + 2 t + 1 D(t) = 1 – 13 t – 13 t^2 – … – 13 t^26 + 91
 *            t^27 Hence a(0)=1, for 1≤n<27 a(n)=13·∑_{k=0}^{n-1}a(k)+2 (with
 *            +1 when n=27), for n≥27: a(n)=13·∑_{k=1}^{26}a(n-k) – 91·a(n-27)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* allocate array for values up to n */
    seq_t *val = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!val)
        return (seq_t)0;                 /* allocation failed */

    val[0] = (seq_t)1;

    /* pre‑compute up to n */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i <= 26)
        {
            /* compute sum of previous i terms */
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < i; ++j)
                sum += val[j];
            val[i] = (seq_t)13 * sum + (seq_t)2;
        }
        else if (i == 27)
        {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < 27; ++j)
                sum += val[j];
            val[i] = (seq_t)13 * sum + (seq_t)1;
        }
        else /* i >= 28 */
        {
            seq_t sum26 = (seq_t)0;
            for (idx_t j = 1; j <= 26; ++j)
                sum26 += val[i - j];
            val[i] = (seq_t)13 * sum26 - (seq_t)91 * val[i - 27];
        }
    }

    seq_t result = val[n];
    free(val);
    return result;
}
