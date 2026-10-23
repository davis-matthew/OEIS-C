/*
 * A040903  Continued fraction for sqrt(934).
 * https://oeis.org/A040903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction partial quotients for sqrt(934)
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  sqrt(934) has integer part 30 */
    const seq_t D = 934;
    const seq_t A0 = 30;

    /* initial values for the algorithm */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_prev = A0;

    if (n == 0)
        return a_prev;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* m_{k+1} = d_k * a_k - m_k */
        m = d * a_prev - m;
        /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        d = (D - m * m) / d;
        /* a_{k+1} = floor((A0 + m_{k+1}) / d_{k+1}) */
        a_prev = (A0 + m) / d;
    }

    return a_prev;
}
