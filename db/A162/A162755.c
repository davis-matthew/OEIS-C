/*
 * A162755  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9, a(2)=72, a(3)=540;
 *            a(n)=7*a(n-1)+7*a(n-2)-28*a(n-3) for n>=4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[4] = {
        (seq_t)1,
        (seq_t)9,
        (seq_t)72,
        (seq_t)540
    };

    if (n < 4)
        return init[n];

    /* Sliding window: a(n-3), a(n-2), a(n-1), a(n) */
    seq_t m_minus3 = init[0]; /* a0   */
    seq_t m_minus2 = init[1]; /* a1   */
    seq_t m_minus1 = init[2]; /* a2   */
    seq_t curr     = init[3]; /* a3   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 7 * curr + 7 * m_minus1 - 28 * m_minus2;
        m_minus3 = m_minus2;
        m_minus2 = m_minus1;
        m_minus1 = curr;
        curr     = next;
    }
    return curr;
}
