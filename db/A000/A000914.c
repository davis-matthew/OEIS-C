/*
 * A000914  Stirling numbers of the first kind: s(n+2, n).
 * https://oeis.org/A000914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n+1)*n*(n+2)*(3*n+5)/24
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 309, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t q = m * (m + 1) / 2;
    return q * (m + 2) * (3 * m + 5) / 12;
}
