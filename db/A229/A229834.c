/*
 * A229834  Expansion of (1+4*x+x^2) / ((1-x)^3*(1+x)^4).
 * https://oeis.org/A229834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 + n*(n + 5)*(9 - (2*n + 5)*(-1)^n)/48 (OEIS A229834)
 * Range:     i8 n <= 2, i16 n <= 22, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                         /* cast index to seq_t          */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1; /* (-1)^n : 1 for even, -1 for odd */
    seq_t smul = (2 * nn + 5) * sign;             /* (2*n + 5)*(-1)^n             */
    seq_t t = 9 - smul;                           /* 9 - (2*n + 5)*(-1)^n          */
    seq_t numerator = nn * (nn + 5) * t;          /* n*(n+5)*t                     */
    seq_t result = 1 + numerator / 48;            /* final value                   */
    return result;
}
