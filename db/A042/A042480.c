/*
 * A042480  Numerators of continued fraction convergents to sqrt(768).
 * https://oeis.org/A042480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n‑th continued‑fraction convergent to
 *            sqrt(768) * (where the continued fraction has partial quotients
 *            : 27,1,2,2,13,2,2,1,54 repeating)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(768) */
    const uint64_t D   = 768;      /* m*m < D holds at every step, so uint64_t is safe */
    const uint64_t A0  = 27;       /* floor(sqrt(768)) */

    /* Continued‑fraction state (m,d) for the current step */
    uint64_t m = 0;
    uint64_t d = 1;

    /* Convergent numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;

    for (idx_t i = 0; i <= n; ++i) {
        uint64_t a_k;

        /* Partial quotient a_k */
        if (i == 0)
            a_k = A0;
        else
            a_k = (A0 + m) / d;   /* floor((a0 + m)/d) */

        /* Compute numerator p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t pk = (seq_t)a_k * p_prev1 + p_prev2;

        if (i == n)
            return pk;

        /* Update CF state for next step: m_{k+1}, d_{k+1} */
        uint64_t m_next = d * a_k - m;
        /* d_next = (D - m_next^2) / d;   (integer division guaranteed) */
        uint64_t d_next = (D - m_next * m_next) / d;

        /* Shift convergent history */
        p_prev2 = p_prev1;
        p_prev1 = pk;

        /* Prepare for next iteration */
        m = m_next;
        d = d_next;
    }

    /* Should never reach here */
    return 0;
}
