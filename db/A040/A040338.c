/*
 * A040338  Continued fraction for sqrt(357).
 * https://oeis.org/A040338
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

/* Continued fraction for sqrt(357).
   Computes the (n+1)-th partial quotient a(n) where a(0)=floor(sqrt(357))=18.
   The recurrence:
       m_{k+1} = d_k * a_k - m_k
       d_{k+1} = (357 - m_{k+1}^2) / d_k
       a_{k+1} = floor( (18 + m_{k+1}) / d_{k+1} )
   with m_0=0, d_0=1, a_0=18. */
seq_t a(idx_t n)
{
    const seq_t a0 = 18;          /* floor(sqrt(357)) */
    const seq_t D  = 357;         /* the square‑free part */

    if (n == 0)
        return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;   /* floor division */
        m = m_next;
        d = d_next;
        a = a_next;
    }
    return a;
}
