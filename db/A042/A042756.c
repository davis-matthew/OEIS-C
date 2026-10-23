/*
 * A042756  Numerators of continued fraction convergents to sqrt(909).
 * https://oeis.org/A042756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = cf_n * a(n-1) + a(n-2) where a(0) = 30, a(1) = 181 cf_n
 *            are the continued fraction partial quotients of sqrt(909)
 *            computed by the standard algorithm: m_{k+1} = d_k * a_k - m_k,
 *            d_{k+1} = (909 - m_{k+1}^2) / d_k, a_{k+1} = floor((30 +
 *            m_{k+1}) / d_{k+1})
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t D   = 909;
    const idx_t a0  = 30;

    if (n == 0)
        return (seq_t)a0;          /* a(0) = 30 */

    /* Initial values for the convergent recurrence:
        p_{-1} = 1,  p_0 = a0   (numerators of convergents) */
    seq_t p_prevprev = 1;          /* p_{-1} */
    seq_t p_prev     = (seq_t)a0;  /* p_0 */

    /* Variables for generating the continued‑fraction coefficients of sqrt(909) */
    idx_t m     = 0;               /* m_0   */
    idx_t d     = 1;               /* d_0   */
    idx_t a_prev = a0;             /* a_0   */

    seq_t p_cur;
    for (idx_t k = 1; k <= n; ++k) {
        /* Next continued‑fraction coefficient a_k */
        idx_t m_new = d * a_prev - m;
        idx_t d_new = (D - m_new * m_new) / d;
        idx_t a_new = (a0 + m_new) / d_new;

        /* Numerator recurrence:  p_k = a_k * p_{k-1} + p_{k-2} */
        p_cur = (seq_t)a_new * p_prev + p_prevprev;

        /* Shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_cur;

        m     = m_new;
        d     = d_new;
        a_prev = a_new;
    }

    return p_prev;  /* p_n */
}
