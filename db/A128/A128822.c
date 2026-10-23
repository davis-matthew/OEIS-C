/*
 * A128822  Number of solutions to x/3 + y/4 + z/6 < n with x,y,z>=1 .
 * https://oeis.org/A128822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^3-15*n^2+6*n-1
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t before any arithmetic to avoid mixing types */
    seq_t N = (seq_t)n;
    /* compute polynomial: 12*N^3 - 15*N^2 + 6*N - 1 */
    seq_t term = (seq_t)12 * N * N * N
               - (seq_t)15 * N * N
               + (seq_t)6  * N
               - (seq_t)1;
    return term;
}
