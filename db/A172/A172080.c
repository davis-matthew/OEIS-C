/*
 * A172080  a(n) = n*(12*n^3 + 10*n^2 - 9*n - 7)/6.
 * https://oeis.org/A172080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(12*n^2 - 2*n - 7)/6
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = n;
    seq_t t1  = N * (N + 1);                     /* n*(n+1)  */
    seq_t t2  = 12 * N * N - 2 * N - 7;           /* 12n^2-2n-7 */
    seq_t res = t1 * t2 / 6;                      /* divide by 6 */
    return res;
}
