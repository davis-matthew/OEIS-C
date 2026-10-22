/*
 * A007758  a(n) = 2^n*n^2.
 * https://oeis.org/A007758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * n^2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    return (n_val * n_val) << n;
}
