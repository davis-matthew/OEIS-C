/*
 * A005563  a(n) = n*(n+2) = (n+1)^2 - 1.
 * https://oeis.org/A005563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return m * (m + 2);
}
