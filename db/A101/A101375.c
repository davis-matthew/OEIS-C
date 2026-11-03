/*
 * A101375  a(n) = n*(n+1)*(n^2-2*n+2)/2.
 * https://oeis.org/A101375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2-2*n+2)/2
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 976 (2026-11-01)
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
