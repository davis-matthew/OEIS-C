/*
 * A169576  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 2 + 33 * Σ_{k=0}^{n-1} a(k) for 1 ≤ n ≤ 34 a(n)
 *            = 33*a(n-1) + 33*a(n-2) + … + 33*a(n-34) – 561*a(n-35) for n ≥
 *            35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute the first 35 terms (indices 0..34) */
    seq_t v[35];
    v[0] = (seq_t)1;
    seq_t cum = v[0];          /* cumulative sum Σ_{k=0}^{i-1} a(k) */
    for (idx_t i = 1; i <= 34 && i <= n; ++i) {
        v[i] = (seq_t)2 + (seq_t)33 * cum;
        cum += v[i];
    }
    if (n <= 34)
        return v[n];

    /* set up a circular buffer of the last 35 values */
    seq_t buf[35];
    for (idx_t i = 0; i < 35; ++i)
        buf[i] = v[i];

    seq_t total = cum;         /* sum of all 35 terms in buf */
    idx_t idx = 0;              /* position of a_{i-35} in buf */

    for (idx_t i = 35; ; ++i) {
        seq_t oldest = buf[idx];
        seq_t sum34  = total - oldest;          /* a_{i-1}..a_{i-34} */
        seq_t val    = (seq_t)33 * sum34 - (seq_t)561 * oldest;

        /* update buffer and total for next iteration */
        total = total - oldest + val;
        buf[idx] = val;
        if (i == n)
            return val;

        idx = (idx + 1) % 35;
    }
}
