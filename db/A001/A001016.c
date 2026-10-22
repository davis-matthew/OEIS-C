/*
 * A001016  Eighth powers: a(n) = n^8.
 * https://oeis.org/A001016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x4 = x2 * x2;
    return x4 * x4;
}
