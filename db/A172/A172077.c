/*
 * A172077  a(n) = n*(n+1)*(7*n^2 - n - 4)/4.
 * https://oeis.org/A172077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n^2 - n - 4)/4
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 221, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t N = n;                       /* convert index to seq_t */
    seq_t t   = N * (N + 1);            /* n*(n+1) */
    seq_t part = 7 * N * N - N - 4;     /* 7*n^2 - n - 4 (positive for n>0) */
    seq_t res  = t * part / 4;          /* final value */
    return res;
}
