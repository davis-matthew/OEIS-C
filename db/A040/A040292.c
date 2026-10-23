/*
 * A040292  Continued fraction for sqrt(310).
 * https://oeis.org/A040292
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

/* Continued fraction for sqrt(310).  Computes the n‑th partial quotient
   of the continued fraction expansion of √310.  The algorithm uses
   the standard recurrence
        m_{k+1} = d_k*a_k - m_k
        d_{k+1} = (D - m_{k+1}^2) / d_k
        a_{k+1} = ⌊(a₀ + m_{k+1}) / d_{k+1}⌋
   where D = 310 and a₀ = ⌊√310⌋ = 17.
   It runs in O(n) time and is correct for all n ≥ 0. */
seq_t a(idx_t n)
{
    /* constants for D = 310 and a0 = floor(sqrt(310)) = 17 */
    static const seq_t D  = (seq_t)310;
    static const seq_t a0 = (seq_t)17;

    if (n == 0U)
        return a0;

    /* initial state of the continued fraction algorithm */
    seq_t m = 0;    /* m0 */
    seq_t d = 1;    /* d0 */
    seq_t a_curr = a0; /* a0 */

    /* iterate forward until the desired index is reached */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute next m, d and a */
        seq_t m_next = d * a_curr - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;   /* floor division */

        /* advance to the next step */
        m = m_next;
        d = d_next;
        a_curr = a_next;
    }

    return a_curr;
}
