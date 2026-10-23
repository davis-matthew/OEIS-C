/*
 * A106157  Expansion of g.f. (1-x-x^3+x^4-2*x^2)/((1-2*x)*(x-1)^2*(x+1)^2).
 * https://oeis.org/A106157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2^(n+1)-(3*n+2)*(-1)^n-9*n-18)/18
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute numerator */
    seq_t num = pow2(n + 1);                     /* 2^(n+1) */
    seq_t sign = ((n & 1) ? -1 : 1);             /* (-1)^n  */
    seq_t tri = (seq_t)3 * (seq_t)n + 2;          /* 3*n+2   */
    num -= tri * sign;                           /* subtract (3n+2)(-1)^n */
    num -= (seq_t)9 * (seq_t)n;                  /* subtract 9*n   */
    num -= 18;                                   /* subtract 18    */
    return num / 18;                             /* divide by 18   */
}
