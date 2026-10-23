/*
 * A017560  a(n) = (12*n + 3)^4.
 * https://oeis.org/A017560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 21, u64 n <= 5461,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m  = (seq_t)12 * n + 3;
    seq_t m2 = m * m;
    return m2 * m2;
}
