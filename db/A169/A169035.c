/*
 * A169035  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 20 * sum_{i=0}^{n-1} a(i) + p_n for n < 24 (p_0 = 1,
 *            p_n = 2 for 1 ≤ n < 24) * a(n) = 20 * sum_{i=1}^{23} a(n-i) -
 *            210 * a(n-24) for n ≥ 24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate cache for a(0)..a(n) */
    seq_t *cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!cache)
        return 0;  /* Allocation failed – return zero as safety */

    seq_t sum_prev  = 0;  /* sum of a(0)..a(i-1) for the i < 24 branches   */
    seq_t sum_last23 = 0; /* sum of the last 23 computed values             */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t val;
        if (i < 24)
        {
            seq_t p = (i == 0) ? (seq_t)1 : (seq_t)2;
            val = (seq_t)20 * sum_prev + p;
        }
        else
        {
            val = (seq_t)20 * sum_last23 - (seq_t)210 * cache[i - 24];
        }

        cache[i] = val;

        /* Update running sums for next iterations */
        sum_prev += val;
        if (i >= 23)
        {
            sum_last23 += val;
            if (i >= 23)      /* i-23 is always valid here */
                sum_last23 -= cache[i - 23];
        }
    }

    seq_t result = cache[n];
    free(cache);
    return result;
}
