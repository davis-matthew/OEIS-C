/*
 * A040532  Continued fraction for sqrt(556).
 * https://oeis.org/A040532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of the continued fraction expansion of sqrt(556)
   using the standard recurrence for quadratic irrationals. */
seq_t a(idx_t n)
{
    /* Constants for D = 556 */
    const seq_t D   = 556;
    const seq_t a0  = 23;   /* floor(sqrt(556)) */

    if (n == 0)
        return a0;

    idx_t i;
    seq_t m = 0;        /* m_0 */
    seq_t d = 1;        /* d_0 */
    seq_t a_cur = a0;   /* a_0    */

    /* Compute terms a_1 … a_n */
    for (i = 1; i <= n; ++i)
    {
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return a_cur;
}
