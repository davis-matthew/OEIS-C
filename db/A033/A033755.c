/*
 * A033755  Expansion of Product_{d | 43} theta_3(q^d).
 * https://oeis.org/A033755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i^2 + 43 j^2 = n} w(i)·w(j) with w(0)=1 and w(x)=2 for
 *            x≠0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t val_n = (seq_t)n;

    /* largest j such that 43·j^2 ≤ n */
    seq_t jlim_sq = val_n / (seq_t)43;
    seq_t jlim = floor_sqrt(jlim_sq);

    for (idx_t j = 0; j <= (idx_t)jlim; ++j)
    {
        seq_t jj = (seq_t)j * (seq_t)j;
        seq_t t  = val_n - (seq_t)43 * jj;
        seq_t s  = floor_sqrt(t);

        if (s * s == t)
        {
            seq_t wj = (j == 0) ? (seq_t)1 : 2;
            seq_t ws = (s == 0) ? (seq_t)1 : 2;
            res += wj * ws;
        }
    }
    return res;
}
