/*
 * A042519  Denominators of continued fraction convergents to sqrt(788).
 * https://oeis.org/A042519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the n‑th convergent to sqrt(788) in
 *            its simple continued fraction expansion. The convergents satisfy
 *            the recurrence p_k = a_k p_{k-1} + p_{k-2}, q_k = a_k q_{k-1} +
 *            q_{k-2}, where a_k are the continued‑fraction partial quotients
 *            of sqrt(788)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const idx_t D   = 788;               /* the square‑free integer */
    const idx_t a0  = 28;                /* floor(sqrt(788)) */
    /* Continued‑fraction state: m_k, d_k, a_k */
    idx_t m   = 0;                      /* m_0 */
    idx_t d   = 1;                      /* d_0 */
    idx_t ak  = a0;                     /* a_0 = 28 */

    /* Recurrence state for p_k/q_k */
    seq_t p_prev2 = 0;                  /* p_{-2} */
    seq_t p_prev1 = 1;                  /* p_{-1} */
    seq_t q_prev2 = 1;                  /* q_{-2} */
    seq_t q_prev1 = 0;                  /* q_{-1} */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t p = (seq_t)ak * p_prev1 + p_prev2;  /* p_k */
        seq_t q = (seq_t)ak * q_prev1 + q_prev2;  /* q_k */
        if (k == n) return q;                   /* answer */

        /* Prepare next partial quotient */
        idx_t m_next  = d * ak - m;
        idx_t d_next  = (D - m_next * m_next) / d;
        idx_t ak_next = (a0 + m_next) / d_next;

        /* Shift recurrence buffers */
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;

        /* Update continued‑fraction parameters */
        m  = m_next;
        d  = d_next;
        ak = ak_next;
    }
    /* Unreachable, but keeps the compiler happy */
    return 0;
}
