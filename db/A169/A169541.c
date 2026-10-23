/*
 * A169541  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) + 46 * Σ_{k=1}^{min(n,33)} a(n-k) – 1081 * a(n-34)
 *            where d(n) = 1 for n = 0 or 34, 2 for 1 ≤ n ≤ 33, 0 otherwise.
 *            This follows from the generating function P(t)/Q(t) with Q(t) =
 *            1 – 46t – … – 46t^33 + 1081t^34
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* ring buffer for the last 34 values a[n-34 .. n-1] */
    seq_t buf[34] = {0};

    seq_t val = 0;
    for (idx_t i = 0; i <= n; ++i) {
        /* d(i) */
        seq_t d;
        if (i == 0 || i == 34) {
            d = 1;
        } else if (i <= 33) {
            d = 2;
        } else {
            d = 0;
        }

        val = d;

        /* 46 * Σ_{k=1}^{min(i,33)} a(i-k) */
        idx_t maxk = (i < 33) ? i : 33;
        for (idx_t k = 1; k <= maxk; ++k) {
            val += (seq_t)46 * buf[(i - k) % 34];
        }

        /* –1081 * a(i-34)  (only for i ≥ 34) */
        if (i >= 34) {
            val -= (seq_t)1081 * buf[(i - 34) % 34];
        }

        /* store a(i) in the ring buffer */
        buf[i % 34] = val;
    }
    return val;
}
