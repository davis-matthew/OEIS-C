/*
 * A049983  a(n) is the number of arithmetic progressions of 2 or more
 *          positive integers, strictly increasing with sum <= n.
 * https://oeis.org/A049983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict increasing arithmetic progressions of
 *            length >= 2 of positive integers with sum <= n
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* maximum length k satisfies k*(k+1)/2 <= n  */
    idx_t kmax = 0;
    for (idx_t k = 2; ; ++k) {
        idx_t tri = (idx_t)k * (k + 1) / 2;
        if (tri > n) break;
        kmax = k;
    }

    for (idx_t k = 2; k <= kmax; ++k) {
        seq_t m = (seq_t)2 * n / k;  /* floor(2n/k) */
        if (m < 2) continue;        /* no valid a for any d */

        idx_t denom = k - 1;
        /* maximum d with m - denom*d >= 2  */
        idx_t dmax = 0;
        if (m >= 2) {
            if (denom > 0)
                dmax = (idx_t)(m - 2) / denom;
        }
        for (idx_t d = 1; d <= dmax; ++d) {
            seq_t rem = m - (seq_t)denom * d;
            seq_t amax = rem / 2;
            if (amax >= 1)
                total += amax;
        }
    }
    return total;
}
