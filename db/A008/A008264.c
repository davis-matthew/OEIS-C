/*
 * A008264  Coordination sequence for tridymite, lonsdaleite, and wurtzite.
 * https://oeis.org/A008264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0)=1; for n>0, a(n) = 42*(n/4)^2 + 21*(n%4)*(n/4) +
 *            [2,4,12,25][n%4]
 * Range:     u8 n <= 9, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t m = n / 4;
    idx_t k = n & 3;          /* n % 4 */
    seq_t m_seq = (seq_t)m;

    static const seq_t tbl[4] = {
        (seq_t)2, (seq_t)4, (seq_t)12, (seq_t)25
    };

    return (seq_t)42 * m_seq * m_seq
         + (seq_t)21 * (seq_t)k * m_seq
         + tbl[k];
}
