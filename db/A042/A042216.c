/*
 * A042216  Numerators of continued fraction convergents to sqrt(634).
 * https://oeis.org/A042216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(634)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case directly */
    if (n == 0)
        return (seq_t)25;        /* first convergent 25/1 */

    /* Initial values for the continued fraction of sqrt(634) */
    const idx_t D = 634;
    const idx_t sqrtD = 25;      /* floor(sqrt(634)) */

    idx_t m = 0;                 /* m_0 */
    idx_t d = 1;                 /* d_0 */
    idx_t aCurr = sqrtD;         /* a_0 */

    seq_t p_prev2 = 1;           /* p_{-1} */
    seq_t p_prev  = 25;          /* p_0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Next partial denominator and associated values */
        m   = d * aCurr - m;                                /* m_i   */
        d   = (D - m * m) / d;                              /* d_i   */
        aCurr = (sqrtD + m) / d;                            /* a_i   */

        /* Compute next numerator p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t p_new = p_prev * (seq_t)aCurr + p_prev2;

        /* Shift for next iteration */
        p_prev2 = p_prev;
        p_prev  = p_new;
    }
    return p_prev;
}
