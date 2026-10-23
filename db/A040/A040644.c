/*
 * A040644  Continued fraction for sqrt(670).
 * https://oeis.org/A040644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Continued fraction for sqrt(670).
 * a(0) = floor(sqrt(670)) = 25.
 * For k ≥ 1:
 *    m_k   = d_{k-1} * a_{k-1} - m_{k-1}
 *    d_k   = (670 - m_k^2) / d_{k-1}
 *    a_k   = floor((25 + m_k) / d_k)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;

    const seq_t N   = (seq_t)670;   /* the radicand */
    const seq_t A0  = (seq_t)25;    /* floor(sqrt(N)) */

    seq_t m    = (seq_t)0;   /* m_{k-1} */
    seq_t d    = (seq_t)1;   /* d_{k-1} */
    seq_t a_prev = A0;       /* a_{k-1} */
    seq_t a_cur  = 0;        /* a_k */

    for (idx_t k = 1; k <= n; ++k) {
        /* m_k = d_{k-1} * a_{k-1} - m_{k-1} */
        m = d * a_prev - m;

        /* d_k = (N - m_k^2) / d_{k-1} */
        d = (N - m * m) / d;

        /* a_k = floor((A0 + m_k) / d_k) */
        a_cur = (A0 + m) / d;

        a_prev = a_cur;   /* prepare for next iteration */
    }

    return a_cur;
}
