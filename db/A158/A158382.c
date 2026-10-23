/*
 * A158382  a(n) = 625*n^2 + 2*n.
 * https://oeis.org/A158382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 625*n^2 + 2*n
 * Range:     u8 none, u16 n <= 10, u32 n <= 2621, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return (seq_t)625 * nn * nn + (seq_t)2 * nn;
}
