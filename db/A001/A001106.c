/*
 * A001106  9-gonal (or enneagonal or nonagonal) numbers: a(n) = n*(7*n-5)/2.
 * https://oeis.org/A001106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(7*n-5)/2
 * Range:     u8 n <= 8, u16 n <= 137, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return x * (7 * x - 5) / 2;
}
