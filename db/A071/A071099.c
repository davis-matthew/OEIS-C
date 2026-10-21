/*
 * A071099  a(n) = (n-1)*(n+3) - 2^n + 4.
 * https://oeis.org/A071099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)*(n+3) - 2^n + 4
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* (n+1)^2 - 2^n is equivalent to (n-1)*(n+3) - 2^n + 4 */
    seq_t w = (seq_t)(n + 1);  /* n+1 */
    seq_t w2 = w * w;          /* (n+1)^2 */
    seq_t p2 = pow2(n);        /* 2^n   */
    return w2 - p2;
}
