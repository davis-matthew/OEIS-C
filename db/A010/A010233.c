/*
 * A010233  Continued fraction for sqrt(191).
 * https://oeis.org/A010233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction coefficients of sqrt(191)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t a0 = 13;       /* floor(sqrt(191)) */
    static const seq_t D  = 191;
    if (n == 0) return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t q = a0;                     /* current coefficient a_k */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * q - m;          /* m_{k+1} = d_k * a_k - m_k */
        seq_t d_next = (D - m_next * m_next) / d; /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        seq_t q_next = (a0 + m_next) / d_next;    /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */

        if (i == n) return q_next;        /* return desired term */

        m = m_next;
        d = d_next;
        q = q_next;
    }

    return a0; /* unreachable, keeps the function well formed */
}
