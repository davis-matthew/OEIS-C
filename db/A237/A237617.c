/*
 * A237617  a(n) = n*(n + 1)*(17*n - 14)/6.
 * https://oeis.org/A237617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(17*n - 14)/6
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 675, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * (nn + 1) * (17 * nn - 14) / 6;
}
