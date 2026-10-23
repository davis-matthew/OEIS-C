/*
 * A167875  One third of product plus sum of three consecutive nonnegative
 *          integers; a(n)=(n+1)(n^2+2n+3)/3.
 * https://oeis.org/A167875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2+2*n+3)/3
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Compute N^2 + 2*N + 3 */
    seq_t tmp = N * N;        /* N^2 */
    tmp = tmp + 2 * N;        /* N^2 + 2*N */
    tmp = tmp + 3;            /* N^2 + 2*N + 3 */

    /* Multiply by (N+1) and divide by 3 */
    seq_t res = (N + 1) * tmp;  /* (n+1)*(N^2+2*N+3) */
    return res / 3;
}
