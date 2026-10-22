/*
 * A007997  a(n) = ceiling((n-3)(n-4)/6).
 * https://oeis.org/A007997
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = ceiling((n-3)(n-4)/6)
 * Range:     u8 n <= 19, u16 n <= 424, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4) {
        return 0;
    }
    
    seq_t m_minus_3 = (seq_t)(n - 3);
    seq_t m_minus_4 = (seq_t)(n - 4);
    seq_t product = m_minus_3 * m_minus_4;
    
    return (product + 5) / 6;
}
