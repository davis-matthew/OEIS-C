/*
 * A154600  a(n) = 2*n^2 + 22*n + 9.
 * https://oeis.org/A154600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 22*n + 9
 * Range:     u8 n <= 6, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 2 * nn * nn + 22 * nn + 9;
}
