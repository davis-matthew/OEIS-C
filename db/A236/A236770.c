/*
 * A236770  a(n) = n*(n + 1)*(3*n^2 + 3*n - 2)/8.
 * https://oeis.org/A236770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n^2 + 3*n - 2)/8
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 295, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t term1 = N * (N + 1);                     // n*(n+1)
    seq_t term2 = (3 * N * N) + (3 * N) - (seq_t)2; // 3*n^2 + 3*n - 2
    seq_t result = (term1 * term2) / (seq_t)8;       // divide by 8
    return result;
}
