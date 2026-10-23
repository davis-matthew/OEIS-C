/*
 * A040906  Continued fraction for sqrt(937).
 * https://oeis.org/A040906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(937), computed via the standard
   periodic algorithm.  For n = 0 the first coefficient a0 = ⌊√937⌋ = 30.
   For n > 0, a(n) is produced by repeating:
        m_k  = d_{k-1}*a_{k-1} - m_{k-1}
        d_k  = (937 - m_k*m_k) / d_{k-1}
        a_k  = (30 + m_k) / d_k   (floor division)
   where a0 = 30. */
 
seq_t a(idx_t n)
{
    const seq_t a0 = (seq_t)30;        /* floor(sqrt(937))   */
    const seq_t S  = (seq_t)937;       /* the square‑root argument */

    if (n == 0U)
        return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_term = a0;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m_next = d * a_term - m;
        seq_t d_next = (S - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* advance to next step */
        m = m_next;
        d = d_next;
        a_term = a_next;
    }
    return a_term;
}
