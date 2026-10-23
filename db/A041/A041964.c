/*
 * A041964  Numerators of continued fraction convergents to sqrt(505).
 * https://oeis.org/A041964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) are the numerators of the convergents to sqrt(505). They
 *            are generated from the continued‑fraction expansion of sqrt(505)
 *            with the recurrence m_{k+1} = d_k * a_k - m_k d_{k+1} = (505 -
 *            m_{k+1}^2) / d_k a_{k+1} = (22 + m_{k+1}) / d_{k+1} and the
 *            convergent numerators p_{-2}=0, p_{-1}=1 p_k =...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t a0 = 22;               /* floor(sqrt(505)) */
    idx_t m = 0;                       /* m_0 */
    idx_t d = 1;                       /* d_0 */
    idx_t a = a0;                      /* a_0 */

    seq_t prev2 = 0;                   /* p_{-2} */
    seq_t prev1 = 1;                   /* p_{-1} */
    seq_t curr;

    for (idx_t i = 0; i <= n; ++i) {
        curr = (seq_t)a * prev1 + prev2;
        if (i == n) return curr;

        /* shift for next step */
        prev2 = prev1;
        prev1 = curr;

        /* compute next partial quotient a */
        m = d * a - m;
        d = (505 - m * m) / d;
        a = (a0 + m) / d;
    }
    return 0;   /* unreachable, but required for completeness */
}
