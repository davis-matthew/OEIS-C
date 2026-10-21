/*
 * A056108  Fourth spoke of a hexagonal spiral.
 * https://oeis.org/A056108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n^2 + n + 1
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return ((seq_t)3) * nn * nn + nn + 1;
}
