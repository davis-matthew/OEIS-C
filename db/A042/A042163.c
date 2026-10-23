/*
 * A042163  Denominators of continued fraction convergents to sqrt(606).
 * https://oeis.org/A042163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of sqrt(606); The
 *            sequence satisfies the recurrence a(0) = 1, a(1) = 1, a(n) =
 *            c(n)*a(n-1) + a(n-2) (n ≥ 2), where c(n) is the n‑th partial
 *            quotient of the continued‑fraction expansion of √606
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 28, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t sqrtD = (seq_t)24;   /* floor(sqrt(606)) */
    const seq_t D      = (seq_t)606; /* (√606)^2 */

    /* convergent numerators/denominators (p_i, q_i) and CF state (m, d, a) */
    seq_t p_prev2 = (seq_t)0, p_prev1 = (seq_t)1; /* p_{-2}, p_{-1} */
    seq_t q_prev2 = (seq_t)1, q_prev1 = (seq_t)0; /* q_{-2}, q_{-1} */
    seq_t m = (seq_t)0,          /* m_{0}   */
          d = (seq_t)1,          /* d_{0}   */
          a_prev = sqrtD;        /* a_{0}   */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = sqrtD;
        } else {
            /* compute m_i, d_i, a_i */
            seq_t m_i = d * a_prev - m;
            seq_t d_i = (D - m_i * m_i) / d;
            a_i = (sqrtD + m_i) / d_i;

            /* update CF state for next iteration */
            m = m_i;
            d = d_i;
            a_prev = a_i;
        }

        /* compute convergent denominator */
        seq_t q_i = a_i * q_prev1 + q_prev2;

        if (i == n) {
            return q_i;
        }

        /* shift previous values for next step */
        q_prev2 = q_prev1;
        q_prev1 = q_i;

        /* update numerators (needed only for correctness of future steps) */
        seq_t p_i = a_i * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_i;
    }

    /* This point is unreachable but satisfies the compiler */
    return (seq_t)0;
}
