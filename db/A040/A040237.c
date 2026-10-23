/*
 * A040237  Continued fraction for sqrt(253).
 * https://oeis.org/A040237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of √253 computed via the standard algorithm.
   a(0) = floor(√253) = 15.
   For n > 0, a(n) is obtained by repeating the continued–fraction step
   for quadratic surds. */
seq_t a(idx_t n)
{
    const idx_t A0 = 15;            /* floor of sqrt(253) */
    if (n == 0)
        return (seq_t)A0;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_prev = A0;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* m_k   = d_{k-1} * a_{k-1} - m_{k-1} */
        m = d * a_prev - m;
        /* d_k   = (D - m_k^2) / d_{k-1}     */
        d = (253 - m * m) / d;
        /* a_k   = floor((A0 + m_k) / d_k) */
        idx_t a_curr = (A0 + m) / d;

        if (i == n)
            return (seq_t)a_curr;

        a_prev = a_curr;
    }

    /* Should never reach here */
    return (seq_t)0;
}
