/*
 * A224667  Number of 5 X 5 0..n matrices with each 2 X 2 subblock idempotent.
 * https://oeis.org/A224667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2 + 52*n + 140 (valid for all n >= 1)
 * Range:     u8 n <= 1, u16 n <= 121, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;           /* promote index to result type */
    return 4*nn*nn + 52*nn + (seq_t)140;
}
