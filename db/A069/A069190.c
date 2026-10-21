/*
 * A069190  Centered 24-gonal numbers.
 * https://oeis.org/A069190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 - 12*n + 1
 * Range:     u8 n <= 5, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return (seq_t)12 * N * N - (seq_t)12 * N + 1;
}
