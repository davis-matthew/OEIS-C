/*
 * A158556  a(n) = 28*n^2 + 1.
 * https://oeis.org/A158556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*n*n + 1
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return ((seq_t)28 * nn * nn) + 1;
}
