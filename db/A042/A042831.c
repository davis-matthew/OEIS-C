/*
 * A042831  Denominators of continued fraction convergents to sqrt(946).
 * https://oeis.org/A042831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of the continued
 *            fraction expansion of sqrt(946)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* continued fraction data for sqrt(946) */
    const uint64_t S = 946;
    const uint64_t a0 = 30;          /* floor(sqrt(946)) */

    /* handle first convergent */
    if (n == 0) {
        return (seq_t)1;
    }

    /* recurrence for convergents */
    uint64_t m = 0, d = 1, a_val = a0;
    seq_t q_prev2 = 0;          /* q_{-1} */
    seq_t q_prev1 = 1;          /* q_0   */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute the next parameters of the continued fraction */
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (S - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* compute the denominator of the i-th convergent */
        seq_t q_i = (seq_t)a_next * q_prev1 + q_prev2;

        /* update for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q_i;
        m = m_next;
        d = d_next;
        a_val = a_next;
    }

    return q_prev1;
}
