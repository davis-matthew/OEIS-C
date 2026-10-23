/*
 * A042870  Numerators of continued fraction convergents to sqrt(967).
 * https://oeis.org/A042870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of convergents to sqrt(967)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t D    = 967;      /* D = 967 */
    const idx_t a0   = 31;       /* floor(sqrt(967)) */

    /* Continued‑fraction state */
    idx_t m = 0;                  /* m_k in the algorithm */
    idx_t d = 1;                  /* d_k in the algorithm */
    idx_t a_prev = a0;            /* a_k for the current k (starts with a0) */

    /* Convergent (numerator) state: h_{-2} = 0, h_{-1} = 1 */
    seq_t h_prevprev = 0;  /* h_{i-2} */
    seq_t h_prev     = 1;  /* h_{i-1} */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t a_cur;
        if (i == 0) {
            a_cur = a0;          /* a_0 */
        } else {
            /* Compute next continued‑fraction term a_{i} */
            idx_t m_next = d * a_prev - m;
            idx_t d_next = (D - m_next * m_next) / d;
            a_cur = (a0 + m_next) / d_next;

            /* Update state for the next round */
            m = m_next;
            d = d_next;
            a_prev = a_cur;
        }

        /* Numerator recurrence: h_i = a_i * h_{i-1} + h_{i-2} */
        seq_t h_cur = (seq_t)a_cur * h_prev + h_prevprev;
        h_prevprev = h_prev;
        h_prev = h_cur;
    }

    return h_prev;   /* h_n is the answer */
}
