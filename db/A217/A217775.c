/*
 * A217775  a(n) = n*(n+1) + (n+2)*(n+3) + (n+4)*(n+5).
 * https://oeis.org/A217775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 15*n + 26
 * Range:     u8 n <= 6, u16 n <= 145, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return (seq_t)3 * x * x + (seq_t)15 * x + (seq_t)26;
}
