/*
 * A008511  Number of points on surface of 4-dimensional cube.
 * https://oeis.org/A008511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^4 - (n-1)^4 = 8*n + 8*n^3
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 812, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n;   /* cast index to seq_t to avoid type mixing */
    seq_t m2 = m * m;      /* n^2 */
    seq_t m3 = m2 * m;     /* n^3 */
    return 8 * (m3 + m);   /* 8*n^3 + 8*n */
}
