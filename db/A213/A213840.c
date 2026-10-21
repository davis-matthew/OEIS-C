/*
 * A213840  a(n) = n*(1 + n)*(3 - 4*n + 4*n^2)/6.
 * https://oeis.org/A213840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3 - 4*n + 4*n^2)/6
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 182, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t N  = n;
    seq_t t1 = N * (N + 1);          /* n*(n+1) */
    seq_t t2 = 4 * N * N - 4 * N + 3;/* 4n^2-4n+3   */
    return t1 * t2 / 6;
}
