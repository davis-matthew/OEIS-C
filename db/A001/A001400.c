/*
 * A001400  Number of partitions of n into at most 4 parts.
 * https://oeis.org/A001400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(((n+4)^3 + 3*(n+4)^2 - 9*(n+4)*((n+4) mod 2))/144)
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n + 4;
    seq_t m_sq = m * m;
    seq_t m_cu = m_sq * m;
    seq_t mod_term = m & 1;
    seq_t numerator = m_cu + 3 * m_sq - 9 * m * mod_term + 72;
    return numerator / 144;
}
