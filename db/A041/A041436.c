/*
 * A041436  Numerators of continued fraction convergents to sqrt(234).
 * https://oeis.org/A041436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(234)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t D   = 234;            /* D = 234 */
    const seq_t a0  = 15;             /* floor(sqrt(234)) */

    seq_t p_prevprev = 0;             /* p_{-2} */
    seq_t p_prev    = 1;              /* p_{-1} */
    seq_t p          = 0;             /* current numerator */

    /* state for generating continued‑fraction coefficients */
    seq_t m = 0;                      /* m_0 */
    seq_t d = 1;                      /* d_0 */
    seq_t a_coeff = a0;               /* a_0 */

    idx_t k;
    for (k = 0; k <= n; ++k) {
        /* compute p_k = a_k * p_{k-1} + p_{k-2} */
        p = a_coeff * p_prev + p_prevprev;
        if (k == n)
            break;

        /* generate next coefficients */
        seq_t m_next = d * a_coeff - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* shift state */
        m = m_next;
        d = d_next;
        a_coeff = a_next;

        p_prevprev = p_prev;
        p_prev = p;
    }

    return p;
}
