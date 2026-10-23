/*
 * A198961  a(n) = 4*9^n-1.
 * https://oeis.org/A198961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*9^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) pow *= 9;
    return 4 * pow - 1;
}
