/*
 * A121628  Nonnegative k such that 3*k + 1 is a perfect cube.
 * https://oeis.org/A121628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-1)*(3*n^2-3*n+1)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 782, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;
    seq_t s = (seq_t)3 * nn * nn - (seq_t)3 * nn + (seq_t)1;
    return (seq_t)3 * n1 * s;
}
