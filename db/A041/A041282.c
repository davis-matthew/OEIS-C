/*
 * A041282  Numerators of continued fraction convergents to sqrt(154).
 * https://oeis.org/A041282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A041282: Numerators of continued fraction convergents to sqrt(154).
 *
 * Recurrence:
 *   p_{-1} = 1,          p_0 = 12
 *   For k ≥ 1:
 *     m_k    = d_{k-1} * a_{k-1} - m_{k-1}
 *     d_k    = (154 - m_k^2) / d_{k-1}
 *     a_k    = floor((12 + m_k) / d_k)
 *     p_k    = a_k * p_{k-1} + p_{k-2}
 *
 *   Initial values: a_0 = 12, m_0 = 0, d_0 = 1, a_{-1} = 12 (used for k=1).
 */
seq_t a(idx_t n)
{
    const idx_t a_floor = 12u;   /* floor(sqrt(154)) */
    const idx_t D       = 154u;

    seq_t p_prev2 = 1;   /* p_{-1} */
    seq_t p_prev1 = a_floor; /* p_0 */
    if (n == 0)
        return p_prev1;

    idx_t m = 0;         /* m_0 */
    idx_t d = 1;         /* d_0 */
    idx_t a_prev = a_floor; /* a_{0} used in the first step */

    for (idx_t k = 1; k <= n; ++k) {
        /* continued‑fraction state transition */
        idx_t m_next = d * a_prev - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a_floor + m_next) / d_next;

        /* convergent numerator */
        seq_t p_next = (seq_t)a_next * p_prev1 + p_prev2;

        /* roll state for next iteration */
        p_prev2  = p_prev1;
        p_prev1  = p_next;
        m        = m_next;
        d        = d_next;
        a_prev   = a_next;
    }
    return p_prev1;
}
