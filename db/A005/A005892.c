/*
 * A005892  Truncated square numbers: 7*n^2 + 4*n + 1.
 * https://oeis.org/A005892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 + 4*n + 1
 * Range:     u8 n <= 5, u16 n <= 96, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return 7 * m * m + 4 * m + 1;
}
