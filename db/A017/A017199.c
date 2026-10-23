/*
 * A017199  a(n) = (9*n + 3)^3.
 * https://oeis.org/A017199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 3)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute 9*n + 3 in the possibly larger seq_t type */
    seq_t x = (seq_t)9 * n + (seq_t)3;
    /* Return the cube of that value */
    return x * x * x;
}
