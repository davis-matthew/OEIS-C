/*
 * A006564  Icosahedral numbers: a(n) = n*(5*n^2 - 5*n + 2)/2.
 * https://oeis.org/A006564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*(5*n^2 - 5*n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 978, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t m_sq = m * m;
    return m * (5*m_sq - 5*m + 2) / 2;
}
