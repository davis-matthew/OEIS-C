/*
 * A136395  Binomial transform of [1, 3, 4, 3, 2, 0, 0, 0, ...].
 * https://oeis.org/A136395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 17 n^2 + 18 n)/12 + 1
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 280, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n4 = n2 * n2;
    seq_t num = n4 + (seq_t)17 * n2 + (seq_t)18 * N;
    seq_t res = num / (seq_t)12 + (seq_t)1;
    return res;
}
