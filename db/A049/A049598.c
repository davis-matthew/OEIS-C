/*
 * A049598  12 times triangular numbers.
 * https://oeis.org/A049598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*(n+1)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * (nn + 1) * 6;
}
