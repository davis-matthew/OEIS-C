/*
 * A042074  Numerators of continued fraction convergents to sqrt(561).
 * https://oeis.org/A042074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042074: numerators of continued fraction convergents to sqrt(561) */
seq_t a(idx_t n)
{
    /* sqrt(561) has integer part 23. */
    const unsigned D = 561U;
    const unsigned a0 = 23U;      /* floor( sqrt(561) ) */

    seq_t p_prev2 = 0;            /* p_{-2} */
    seq_t p_prev1 = 1;            /* p_{-1} */
    seq_t p_cur;

    unsigned m = 0;               /* initial m_{-1} */
    unsigned d = 1;               /* initial d_{-1} */
    unsigned a_term = a0;         /* a_0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* p_k = a_k * p_{k-1} + p_{k-2} */
        p_cur = (seq_t)a_term * p_prev1 + p_prev2;
        if (k == n) return p_cur;

        /* compute next m, d, a using the continued‑fraction recurrence */
        unsigned m_next = d * a_term - m;
        unsigned d_next = (D - m_next * m_next) / d;
        a_term = (a0 + m_next) / d_next;

        /* shift for next iteration */
        m = m_next;
        d = d_next;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    return 0;   /* unreachable, but keeps the compiler happy */
}
