/*
 * A042091  Denominators of continued fraction convergents to sqrt(569).
 * https://oeis.org/A042091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(569)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* constants for sqrt(569) */
    const seq_t N = 569;
    const seq_t a0 = 23;          /* floor(sqrt(569)) */

    /* recurrence variables for convergents q_{-2} = 1, q_{-1} = 0 */
    seq_t prev2_q = 1;   /* q_{-2} */
    seq_t prev1_q = 0;   /* q_{-1} */

    /* variables for continued fraction generation */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = a0;  /* a_0 */

    for (idx_t i = 0; i <= n; ++i) {
        if (i != 0) {
            /* generate next partial quotient a_{i} */
            seq_t m_next = d * a_k - m;
            seq_t d_next = (N - m_next * m_next) / d;
            a_k = (a0 + m_next) / d_next;
            m = m_next;
            d = d_next;
        }

        /* compute q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t q_i = a_k * prev1_q + prev2_q;

        /* shift for next iteration */
        prev2_q = prev1_q;
        prev1_q = q_i;
    }

    /* after loop, prev1_q holds q_n */
    return prev1_q;
}
