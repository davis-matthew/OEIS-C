/*
 * A001303  Stirling numbers of first kind, s(n+3, n), negated.
 * https://oeis.org/A001303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3, 4)*binomial(n+3, 2)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n + 3;
    seq_t c2 = m * (m - 1) / 2;
    seq_t c4 = m;
    c4 = c4 * (m - 1) / 2;
    c4 = c4 * (m - 2) / 3;
    c4 = c4 * (m - 3) / 4;
    return c4 * c2;
}
