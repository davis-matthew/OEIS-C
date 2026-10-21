/*
 * A067692  a(n) = Sum_{0 < d <= t <= n, d|n, t|n} d*t.
 * https://oeis.org/A067692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(sigma_1(n)^2 + sigma_2(n))
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the undefined case n == 0 safely */
    if (n == 0) return 0;

    seq_t sigma1 = 0;
    seq_t sigma2 = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t dseq = (seq_t)d;
            idx_t q = n / d;
            seq_t qseq = (seq_t)q;

            sigma1 += dseq;
            sigma2 += dseq * dseq;

            if (q != d) {
                sigma1 += qseq;
                sigma2 += qseq * qseq;
            }
        }
    }

    return (sigma1 * sigma1 + sigma2) / 2;
}
