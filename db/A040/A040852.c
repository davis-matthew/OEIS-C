/*
 * A040852  Continued fraction for sqrt(882).
 * https://oeis.org/A040852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the n-th term of the continued fraction expansion of
 *            sqrt(882)
 * Range:     u8 n <= 1, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t a0 = (seq_t)29;          /* floor(sqrt(882)) */
    if (n == 0)
        return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_prev = a0;
    seq_t a_cur = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next values of m and d. */
        m = d * a_prev - m;                           /* m_{k+1} = d_k*a_k - m_k */
        d = ((seq_t)882 - m * m) / d;                 /* d_{k+1} = (N - m_{k+1}^2) / d_k */

        /* Compute next continued‑fraction term. */
        a_cur = (a0 + m) / d;                          /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */

        if (i == n)
            return a_cur;
        a_prev = a_cur;
    }
    /* n==0 case already handled, so this line is never reached. */
    return a_cur;
}
