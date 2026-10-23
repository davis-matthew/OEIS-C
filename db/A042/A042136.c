/*
 * A042136  Numerators of continued fraction convergents to sqrt(593).
 * https://oeis.org/A042136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            for sqrt(593) a(0) = 24 Recurrence: a0 = floor(sqrt(593)) = 24.
 *            For k ≥ 0, generate partial quotients a_k by m_{k+1} = d_k * a_k
 *            - m_k d_{k+1} = (593 - m_{k+1}^2) / d_k a_{k+1} = floor((a0 +
 *            m_{k+1}) / d_{k+1}) Convergents satisfy p...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* 593 is not a perfect square.  */
    static const idx_t D = 593;
    const idx_t a0 = 24;                   /* floor(sqrt(D)) */

    if (n == 0) {
        return (seq_t)a0;
    }

    /* Convergent numerators: p_{-2}=0, p_{-1}=1, p_0=a0. */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t p_curr  = (seq_t)a0;              /* p_0 */

    /* State of the continued fraction expansion. */
    idx_t m = 0;                /* m_0 */
    idx_t d = 1;                /* d_0 */
    idx_t a_k = a0;             /* a_k = a_{k-1} for the next step */

    idx_t i = 1;
    while (i <= n) {
        /* Next values of m, d, and a (partial quotient). */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Compute next numerator. */
        seq_t a_next_seq = (seq_t)a_next;
        seq_t p_next = a_next_seq * p_curr + p_prev1;

        /* Shift variables for the next iteration. */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        p_curr  = p_next;

        m = m_next;
        d = d_next;
        a_k = a_next;

        ++i;
    }

    return p_curr;   /* p_n */
}
