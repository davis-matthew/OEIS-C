/*
 * A108520  Expansion of 1/(1+2*x+2*x^2).
 * https://oeis.org/A108520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   pow_neg4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_neg4.h"
/*  OEIS A108520 – Expansion of 1/(1+2*x+2*x^2).
    Recurrence:  a(n) = -2*(a(n-1)+a(n-2)),   a(0)=1, a(1)=-2.
    Equivalent closed form:
        a(4k)   = (-4)^k
        a(4k+1) = -2*(-4)^k
        a(4k+2) =  2*(-4)^k
        a(4k+3) = 0
*/

seq_t a(idx_t n)
{
    /* Work out n = 4*k + r with 0 <= r < 4 */
    idx_t r = n & 3U;    /* n % 4 */
    idx_t k = n >> 2U;   /* n / 4 */

    seq_t pow = pow_neg4(k);   /* (-4)^k */

    if (r == 0U)
        return pow;            /* a(4k)   */
    else if (r == 1U)
        return -2 * pow;       /* a(4k+1) */
    else if (r == 2U)
        return  2 * pow;       /* a(4k+2) */
    else
        return (seq_t)0;       /* a(4k+3)=0 */
}
