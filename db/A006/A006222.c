/*
 * A006222  a(n) = 11*n^2 + 11*n + 3.
 * https://oeis.org/A006222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*n^2 + 11*n + 3
 * Range:     u8 n <= 4, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 11 * nn * nn + 11 * nn + 3;
}
