/*
 * A042609  Denominators of continued fraction convergents to sqrt(833).
 * https://oeis.org/A042609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of sqrt(833)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Below are the parameters for the continued fraction of sqrt(833) */
    const idx_t D   = 833;   /* The radicand */
    const idx_t a0  = 28;    /* floor(sqrt(833)) */

    /* Previous two convergents: p_{-2}, p_{-1}  and  q_{-2}, q_{-1} */
    seq_t p_minus2 = 0, p_minus1 = 1;
    seq_t q_minus2 = 1, q_minus1 = 0;

    /* Variables for the quadratic–irrational iteration */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;  /* a_k in the continued fraction */

    seq_t p, q;
    idx_t k;
    for (k = 0; k <= n; ++k) {
        /* p_k = a_k * p_{k-1} + p_{k-2}  (same for q) */
        p = (seq_t)a * p_minus1 + p_minus2;
        q = (seq_t)a * q_minus1 + q_minus2;

        if (k == n)
            return q;          /* desired denominator */

        /* Shift previous values for next iteration */
        p_minus2 = p_minus1;  p_minus1 = p;
        q_minus2 = q_minus1;  q_minus1 = q;

        /* Compute next values of the continued fraction */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - (m_next * m_next)) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;  d = d_next;  a = a_next;
    }

    /* This point is unreachable; return 0 to silence compiler warnings */
    return 0;
}
