/*
 * A157375  a(n) = 2401*n^2 - 980*n + 99.
 * https://oeis.org/A157375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2401*n^2 - 980*n + 99
 * Range:     u8 none, u16 n <= 5, u32 n <= 1337, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 2401 * nn * nn - 980 * nn + 99;
}
