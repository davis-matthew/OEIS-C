/*
 * A105746  a(n) = minimal c>0 such that (n+1)^2+4*n*c = d^2 is a square.
 * https://oeis.org/A105746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal c>0 such that (n+1)^2 + 4*n*c is a perfect
 *            square. The implementation searches for the smallest m = d-(n+1)
 *            that makes m*(m + 2*(n+1)) divisible by 4*n, for which the
 *            resulting c is minimal
 * Range:     u8 n <= 6, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul128.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t to perform arithmetic in seq_t. */
    seq_t n_seq = (seq_t)n;
    seq_t two_n_plus_two = 2 * ((seq_t)(n + 1));    /* 2*(n+1) */
    seq_t four_n = 4 * n_seq;                      /* 4*n */

    for (idx_t m_idx = 1; ; ++m_idx) {
        seq_t m = (seq_t)m_idx;
        /* numerator = m * (m + 2*(n+1)) using mul128 to avoid overflow. */
        seq_t numerator = mul128(m, m + two_n_plus_two);

        if (numerator % four_n == 0) {
            return numerator / four_n;             /* minimal c found */
        }
    }
}
