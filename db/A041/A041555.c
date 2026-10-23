/*
 * A041555  Denominators of continued fraction convergents to sqrt(295).
 * https://oeis.org/A041555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   denominators of continued fraction convergents to sqrt(295)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t D    = (seq_t)295;
    const seq_t a0   = (seq_t)17;          /* floor(sqrt(295)) */

    /* Continued‑fraction data */
    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a_cf= a0;        /* a_0 */

    /* Denominator recurrence: q_{-2}=1, q_{-1}=0 */
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;
    seq_t q_curr  = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* q_i = a_i * q_{i-1} + q_{i-2} */
        q_curr = a_cf * q_prev1 + q_prev2;
        if (i == n)
            return q_curr;

        /* Next continued‑fraction term */
        seq_t m_next = d * a_cf - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Prepare for next iteration of denominator recurrence */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;

        m   = m_next;
        d   = d_next;
        a_cf= a_next;
    }
    return 0;   /* unreachable */
}
