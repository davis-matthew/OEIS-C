/*
 * A140683  a(n) = 3*(-1)^(n+1)*2^n - 1.
 * https://oeis.org/A140683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*(-1)^(n+1)*2^n - 1
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);
    if ((n & 1) == 0)          /* n even → (-1)^(n+1) = -1 */
        val = -val;
    val = val * 3;              /* multiply by 3 */
    return val - 1;             /* subtract 1 */
}
