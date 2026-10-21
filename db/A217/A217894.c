/*
 * A217894  50k^2-20k-23 interleaved with 50k^2+30k+17 for k=>0.
 * https://oeis.org/A217894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (10*n*(5*n-4) - 75*(-1)^n - 1)/4 - 4 Recurrence: a(n) =
 *            2*a(n-1) - 2*a(n-3) + a(n-4) Initial values: a(0) = -23, a(1) =
 *            17, a(2) = 7, a(3) = 97
 * Range:     i8 n <= 2, i16 n <= 26, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use the closed form; works for any integer n >= 0   */
    seq_t tn   = (seq_t)n;                           /* cast index to seq_t   */
    seq_t term = 10*tn*(5*tn-4);                     /* 10*n*(5*n-4)          */
    seq_t sign = (n & 1) ? - (seq_t)1 : (seq_t)1;    /* (-1)^n                 */
    term = (term - 75*sign - 1) / 4 - 4;             /* divide by 4 and subtract 4   */
    return term;
}
