/*
 * A103220  a(n) = n*(n+1)*(3*n^2+n-1)/6.
 * https://oeis.org/A103220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(3*n^2+n-1)/6
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 237, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = N * (N + 1);        /* N*(N+1) */
    term *= ( (seq_t)3 * N * N + N - 1 ); /* * (3*N^2 + N - 1) */
    term /= 6;                       /* divide by 6 */
    return term;
}
