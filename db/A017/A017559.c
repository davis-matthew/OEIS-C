/*
 * A017559  a(n) = (12*n + 3)^3.
 * https://oeis.org/A017559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 135, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n * 12 + 3;
    seq_t t2 = t * t;
    return t2 * t;
}
