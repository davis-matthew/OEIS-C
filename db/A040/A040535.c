/*
 * A040535  Continued fraction for sqrt(559).
 * https://oeis.org/A040535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction partial quotients of sqrt(559).
    a(0) = 23.  For n > 0 use the Legendre recurrence:
        m0 = 0, d0 = 1, a0 = 23
        for i = 0 .. n-1:
            m'  = d_i * a_i - m_i
            d'  = (559 - m'^2) / d_i
            a'  = (23 + m') / d'
            m_i = m', d_i = d', a_i = a'
    The returned value is a_n.  (All divisions are exact.) */

seq_t a(idx_t n)
{
    const seq_t S     = 559;      /* the integer under the root */
    const seq_t A0    = 23;       /* floor(sqrt(559)) */

    if (n == 0) return A0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = A0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a - m;
        seq_t d_next = (S - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
    return a;
}
