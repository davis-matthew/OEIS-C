/*
 * A040792  Continued fraction for sqrt(821).
 * https://oeis.org/A040792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction coefficients for sqrt(821)
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* D = 821 is squarefree; a0 = floor(sqrt(821)) = 28 */
    const seq_t D  = 821;
    const seq_t a0 = 28;

    if (n == 0)
        return a0;

    seq_t m = 0;          /* current m_k   */
    seq_t d = 1;          /* current d_k   */
    seq_t a_curr = a0;    /* a_k           */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a_curr - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_curr = a_next;
    }
    return a_curr;
}
