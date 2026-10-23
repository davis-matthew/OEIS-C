/*
 * A042787  Denominators of continued fraction convergents to sqrt(924).
 * https://oeis.org/A042787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(924)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t D = 924;          /* constant under the square root */
    const idx_t a0 = 30;          /* floor(sqrt(924)) */

    /* Initial values for the recurrence q_{-2}=1, q_{-1}=0 */
    seq_t q_prev2 = 1;
    seq_t q_prev1 = 0;
    seq_t q_current = 0;

    /* Variables for generating continued‑fraction coefficients a_k */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_k = a0; /* a_0 = floor(sqrt(D)) */

    for (idx_t k = 0; k <= n; ++k) {
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        q_current = (seq_t)a_k * q_prev1 + q_prev2;
        if (k == n) return q_current;

        /* Compute next continued‑fraction coefficient */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Shift for next iteration */
        m = m_next;
        d = d_next;
        a_k = a_next;
        q_prev2 = q_prev1;
        q_prev1 = q_current;
    }

    /* Should never reach here */
    return q_current;
}
