/*
 * A017594  a(n) = (12*n + 6)^2.
 * https://oeis.org/A017594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 6)^2
 * Range:     u8 n <= 0, u16 n <= 20, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n;
    t = ((seq_t)12) * t + ((seq_t)6);
    return t * t;
}
