/*
 * A158562  a(n) = 256*n^2 - 16.
 * https://oeis.org/A158562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*n^2 - 16
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 4096, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 256 * nn * nn - 16;
}
