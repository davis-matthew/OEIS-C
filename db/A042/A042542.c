/*
 * A042542  Numerators of continued fraction convergents to sqrt(800).
 * https://oeis.org/A042542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth convergent to sqrt(800) obtained via
 *            the regular continued fraction expansion of sqrt(800)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t D   = 800;   /* D = 800 */
    const idx_t a0  = 28;   /* floor(sqrt(800)) */

    idx_t m = 0;            /* m_0 */
    idx_t d = 1;            /* d_0 */
    idx_t a_k = a0;         /* a_0 */

    seq_t p_m2 = 0;         /* p_{-2} = 0   */
    seq_t p_m1 = 1;         /* p_{-1} = 1   */
    seq_t p_k;              /* p_k = numerator of k-th convergent */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute p_k = a_k * p_{k-1} + p_{k-2} */
        p_k = (seq_t)a_k * p_m1 + p_m2;
        if (k == n)
            return p_k;

        /* Prepare values for the next iteration (k+1) */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_k = a_next;

        /* Shift the previous numerators for the recurrence */
        p_m2 = p_m1;
        p_m1 = p_k;
    }

    /* The loop always returns before reaching this point. */
    return 0;
}
