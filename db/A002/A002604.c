/*
 * A002604  a(n) = n^6 + 1.
 * https://oeis.org/A002604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x6 = x3 * x3;
    return x6 + 1;
}
