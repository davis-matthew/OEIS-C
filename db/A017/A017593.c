/*
 * A017593  a(n) = 12*n + 6.
 * https://oeis.org/A017593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n + 6
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return ((seq_t)n * 12) + 6;
}
