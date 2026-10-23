/*
 * A024219  a(n) = floor( (2nd elementary symmetric function of S(n))/(first
 *          elementary symmetric function of S(n)) ), where S(n) = {first n+1
 *          positive integers congruent to 1 mod 3}.
 * https://oeis.org/A024219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(9*n^2 + 9*n - 2)/(4*(3*n + 2)) )
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 806, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;                 /* convert index to seq_t   */
    seq_t tmp = 9 * N * N + 9 * N - 2;    /* 9*n^2 + 9*n - 2         */
    seq_t num = N * tmp;                  /* n * (9*n^2 + 9*n - 2)   */
    seq_t den = 4 * (3 * N + 2);           /* 4*(3*n + 2)             */
    return num / den;                     /* integer division (floor)*/
}
