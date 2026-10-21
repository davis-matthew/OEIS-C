/*
 * A224337  Number of idempotent 6 X 6 0..n matrices of rank 5.
 * https://oeis.org/A224337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^5 + 60*n^4 + 120*n^3 + 120*n^2 + 60*n + 6
 * Range:     u8 none, u16 n <= 4, u32 n <= 50, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t x5 = x4 * x;
    return 12 * x5 + 60 * x4 + 120 * x3 + 120 * x2 + 60 * x + 6;
}
