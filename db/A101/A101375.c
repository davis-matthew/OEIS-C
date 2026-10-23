/*
 * A101375  a(n) = n*(n+1)*(n^2-2*n+2)/2.
 * https://oeis.org/A101375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2-2*n+2)/2
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t tri = N * (N + 1) / 2;   /* n*(n+1)/2 */
    seq_t quad = N * N - 2 * N + 2;
    return tri * quad;
}
