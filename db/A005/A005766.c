/*
 * A005766  a(n) = cost of minimal multiplication-cost addition chain for n.
 * https://oeis.org/A005766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = a(n) + n^2, a(2n+1) = a(n) + n(n+2), a(1) = 0
 * Range:     u8 n <= 27, u16 n <= 442, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    idx_t m = n / 2;
    seq_t m_seq = (seq_t)m;
    
    if (n % 2 == 0) {
        return a(m) + m_seq * m_seq;
    } else {
        return a(m) + m_seq * (m_seq + 2);
    }
}
