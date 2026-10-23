/*
 * A042592  Numerators of continued fraction convergents to sqrt(825).
 * https://oeis.org/A042592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the continued
 *            fraction of sqrt(825)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 825 = 3 * 5^2 * 11  */
    const idx_t D  = 825;   /* the constant under the square root   */
    const idx_t a0 = 28;    /* floor(sqrt(825))                     */

    /* p_{-2} = 0,  p_{-1} = 1  */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_cur   = 0;

    /*      m_0 = 0,   d_0 = 1,   a_0 = a0  */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t ak = (k == 0) ? (seq_t)a0 : (seq_t)a;   /* current continued‑fraction term */
        p_cur = ak * p_prev1 + p_prev2;               /* recurrence for numerators   */

        /* shift the two previous numerators for the next step */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;

        if (k == n)
            break;                                   /* reached the desired index */

        /* compute the next continued‑fraction term (a_{k+1}) */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
    return p_cur;
}
