/*
 * A040705  Continued fraction for sqrt(733).
 * https://oeis.org/A040705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(733):
 * a(0) = 27
 * For n > 0 let
 *   m_0 = 0,      d_0 = 1,      a_0 = 27
 *   m_k = d_{k-1} * a_{k-1} - m_{k-1}
 *   d_k = (733 - m_k^2) / d_{k-1}
 *   a_k = (27 + m_k) / d_k          (integer division)
 * Then a(n) = a_n.  This recurrence generates the
 * continued‑fraction coefficients of √733.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)27;

    const seq_t D  = (seq_t)733;
    const seq_t A0 = (seq_t)27;

    seq_t m  = (seq_t)0;
    seq_t d  = (seq_t)1;
    seq_t a_val = A0;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m_next = d * a_val - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;
        m    = m_next;
        d    = d_next;
        a_val = a_next;
    }
    return a_val;
}
