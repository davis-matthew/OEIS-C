/*
 * A217776  a(n) = n*(n+1) + (n+2)*(n+3) + (n+4)*(n+5) + (n+6)*(n+7).
 * https://oeis.org/A217776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 28*n + 68
 * Range:     u8 n <= 4, u16 n <= 124, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 4 * nn * nn + 28 * nn + 68;
}
