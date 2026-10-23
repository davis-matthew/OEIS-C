/*
 * A024305  a(n) = 2*(n+1) + 3*n + ... + (k+1)*(n+2-k), where k =
 *          floor((n+1)/2).
 * https://oeis.org/A024305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)/2) * (-2*((n+1)/2)^2 + 3*n*((n+1)/2) + 9*n + 14) /
 *            6
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t m = (N + 1) / 2;          /* ceil(n/2) */
    seq_t m2 = m * m;               /* m^2 */
    seq_t term1 = 2 * m2;           /* 2*m^2   */
    seq_t term2 = 3 * N * m;        /* 3*n*m   */
    seq_t term3 = 9 * N;            /* 9*n     */
    seq_t sum = term2 + term3 + 14 - term1; /* (-2*m^2 + 3*n*m + 9*n + 14) */
    seq_t numerator = m * sum;
    return numerator / 6;
}
