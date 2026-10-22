/*
 * A000578  The cubes: a(n) = n^3.
 * https://oeis.org/A000578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice, mult
 * Formula:   a(n) = n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return x * x * x;
}
