/*
 * A004188  a(n) = n*(3*n^2 - 1)/2.
 * https://oeis.org/A004188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n^2 - 1)/2
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1420, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t m_sq = m * m;
    
    if (m % 2 == 0) {
        return (m / 2) * (3 * m_sq - 1);
    } else {
        return m * ((3 * m_sq - 1) / 2);
    }
}
