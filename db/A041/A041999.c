/*
 * A041999  Denominators of continued fraction convergents to sqrt(522).
 * https://oeis.org/A041999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(522) */
seq_t a(idx_t n)
{
    /* Continued fraction coefficients for sqrt(522) */
    const idx_t D = 522;          /* D = 522   */
    idx_t m = 0;                  /* m0         */
    idx_t d = 1;                  /* d0         */
    idx_t a_curr = 22;            /* a0 = floor(sqrt(522)) = 22 */

    /*  Recurrence for convergents:
        p_{-2}=0, p_{-1}=1
        q_{-2}=1, q_{-1}=0
        p_k = a_k p_{k-1} + p_{k-2}
        q_k = a_k q_{k-1} + q_{k-2}
    */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;

    idx_t k;
    for (k = 0; k <= n; ++k) {
        seq_t a = (seq_t)a_curr;
        seq_t p = a * p_prev1 + p_prev2;
        seq_t q = a * q_prev1 + q_prev2;
        if (k == n)
            return q;

        /* Prepare next coefficient */
        idx_t m_next = d * a_curr - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (22 + m_next) / d_next;

        /* Shift the recurrence */
        m = m_next;
        d = d_next;
        a_curr = a_next;

        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    /* This point should never be reached for valid n */
    return (seq_t)0;
}
