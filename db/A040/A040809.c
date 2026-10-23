/*
 * A040809  Continued fraction for sqrt(838).
 * https://oeis.org/A040809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = nth partial quotient of the continued fraction for
 *            sqrt(838)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The continued fraction expansion of sqrt(838) has the recurrence:
       m_{k+1} = d_k * a_k - m_k
       d_{k+1} = (838 - m_{k+1}^2) / d_k
       a_{k+1} = floor((28 + m_{k+1}) / d_{k+1})   */
    const seq_t D   = 838;    /* discriminant */
    const seq_t a0  = 28;     /* floor(sqrt(838)) */

    if (n == 0)
        return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* compute the next triple (m, d, a) */
        seq_t m_next = d * a_cur - m;
        seq_t diff   = D - m_next * m_next; /* must be non‑negative */
        seq_t d_next = diff / d;            /* exact division */
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
    return a_cur;
}
