/*
 * A139273  a(n) = n*(8*n - 3).
 * https://oeis.org/A139273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(8*n - 3)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * (nn * (seq_t)8 - (seq_t)3);
}
