/*
 * A041354  Numerators of continued fraction convergents to sqrt(191).
 * https://oeis.org/A041354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(191)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Continued fraction parameters for sqrt(191) */
    const seq_t D   = 191;   /* D = 191 */
    const seq_t a0  = 13;    /* a0 = floor(sqrt(191)) */

    /* Initial state for the continued fraction */
    seq_t m     = 0;         /* m_0 */
    seq_t d     = 1;         /* d_0 */
    seq_t a_cf  = a0;        /* a_k   */

    /* Recurrence for convergents */
    seq_t p_prev2 = 0;       /* p_{k-2} */
    seq_t p_prev1 = 1;       /* p_{k-1} */

    for (idx_t k = 0; ; ++k)
    {
        /* Compute p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p = a_cf * p_prev1 + p_prev2;
        if (k == n)
            return p;

        /* Shift for next step */
        p_prev2 = p_prev1;
        p_prev1 = p;

        /* Update continued‑fraction state for k+1 */
        seq_t m_next = d * a_cf - m;               /* m_{k+1} */
        seq_t d_next = D - m_next * m_next;        /* numerator of d_{k+1} */
        d_next = d_next / d;                       /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        seq_t a_next = (a0 + m_next) / d_next;     /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */

        m     = m_next;
        d     = d_next;
        a_cf  = a_next;
    }
}
