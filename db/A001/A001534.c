/*
 * A001534  a(n) = (9*n+1)*(9*n+8).
 * https://oeis.org/A001534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+1)*(9*n+8)
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t term1 = 9 * m + 1;
    seq_t term2 = 9 * m + 8;
    return term1 * term2;
}
