/*
 * A001015  Seventh powers: a(n) = n^7.
 * https://oeis.org/A001015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x4 = x2 * x2;
    return x4 * x2 * x;
}
