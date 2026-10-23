/*
 * A175848  Partial sums of ceiling(n^2/16).
 * https://oeis.org/A175848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^n ceil(k^2 / 16)
 * Range:     u8 n <= 22, u16 n <= 145, u32 n <= 5906, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)0; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t term = (ii * ii + (seq_t)15) / (seq_t)16;
        sum += term;
    }
    return sum;
}
