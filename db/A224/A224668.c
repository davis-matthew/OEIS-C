/*
 * A224668  Number of 6 X 6 0..n matrices with each 2 X 2 subblock idempotent.
 * https://oeis.org/A224668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n^2 + 96*n + 326
 * Range:     u8 none, u16 n <= 96, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;          /* convert index to sequence type */
    return 6 * N * N + 96 * N + 326;
}
