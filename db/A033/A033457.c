/*
 * A033457  GCD-convolution of squares A000290 with themselves.
 * https://oeis.org/A033457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n+2, d<n+2} d^2 * phi((n+2)/d)
 * Range:     u8 n <= 21, u16 n <= 267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    const idx_t m = n + 2;   /* m = n+2 >= 2 for all n >= 0 */
    seq_t total = 0;

    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            idx_t q = m / d;

            /* d is always a proper divisor (d < m) */
            seq_t d_seq = (seq_t)d;
            seq_t term = d_seq * d_seq;           /* d^2   */
            term *= calc_phi((seq_t)q);           /* *phi(q) */
            total += term;

            /* Include the paired divisor q if it is distinct and proper */
            if (q != d && q < m) {
                seq_t q_seq = (seq_t)q;
                seq_t term_q = q_seq * q_seq;
                term_q *= calc_phi(d_seq);        /* *phi(d) */
                total += term_q;
            }
        }
    }
    return total;
}
