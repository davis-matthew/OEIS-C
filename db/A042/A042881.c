/*
 * A042881  Denominators of continued fraction convergents to sqrt(972).
 * https://oeis.org/A042881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   continued‑fraction denominators for sqrt(972); a(0) = 1, a(1) =
 *            5, and for n ≥ 2: a(n) = A_n*a(n-1)+a(n-2) where A_n are the
 *            partial quotients of the continued fraction of √972
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t D   = 972;    /* the target square root D */
    const idx_t a0  = 31;     /* floor(sqrt(D)) */
    seq_t q_prev_prev = 0;    /* q_{k-2} */
    seq_t q_prev      = 1;    /* q_{k-1} */
    idx_t m = 0;              /* m_i in the continued‑fraction algorithm */
    idx_t d = 1;              /* d_i */
    idx_t a_cur = a0;         /* a_i, start with a_0 */

    seq_t q_current;
    for (idx_t k = 1; k <= n; ++k) {
        /* next partial quotient */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* update state for next iteration */
        m = m_next;
        d = d_next;
        a_cur = a_next;

        /* recurrence for convergent denominators */
        q_current = (seq_t)a_cur * q_prev + q_prev_prev;
        q_prev_prev = q_prev;
        q_prev      = q_current;
    }
    return q_prev;   /* q_n */
}
