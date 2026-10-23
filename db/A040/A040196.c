/*
 * A040196  Continued fraction for sqrt(211).
 * https://oeis.org/A040196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction terms for sqrt(211)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t a0 = (seq_t)14;          /* floor(sqrt(211)) */
    const seq_t D  = (seq_t)211;          /* the square‑free integer */

    if (n == 0)
        return a0;

    seq_t m   = (seq_t)0;
    seq_t d   = (seq_t)1;
    seq_t prev = a0;          /* a_k-1, starts as a0 */

    seq_t curr = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m_next = d * prev - m;                 /* m_{k+1} */
        seq_t d_next = (D - m_next * m_next) / d;    /* d_{k+1} */
        curr = (a0 + m_next) / d_next;               /* a_k   */

        if (i == n)
            return curr;

        m = m_next;
        d = d_next;
        prev = curr;
    }

    /* Should never reach here, but keep compiler happy */
    return curr;
}
