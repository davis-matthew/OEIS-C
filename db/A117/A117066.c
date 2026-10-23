/*
 * A117066  Partial sums of cupolar numbers (1/3)*(n+1)*(5*n^2+7*n+3)
 *          (A096000).
 * https://oeis.org/A117066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)*(5*n+1)/12
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 317, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t xn = (seq_t)n;                /* cast index to seq_t */
    seq_t term = xn * xn * (xn + 1);    /* n^2*(n+1) */
    term *= (5 * xn + 1);               /* * (5*n+1) */
    term /= 12;                         /* division is exact for all n >= 1 */
    return term;
}
