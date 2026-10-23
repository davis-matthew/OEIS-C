/*
 * A174836  a(n) = 1 + ((6*n-1)*2^n + (-1)^n)/3.
 * https://oeis.org/A174836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ((6*n-1)*2^n + (-1)^n)/3
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the n = 0 case where (6*n-1) would underflow. */
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Compute 2^n via the provided utility. */
    seq_t pow2_n = pow2(n);

    /* (6*n-1) is positive for n >= 1, so cast safely. */
    seq_t t = (seq_t)(6 * n - 1);

    /* num = (6*n-1) * 2^n + (-1)^n   (performed only for n >= 1). */
    seq_t num = t * pow2_n;

    /* Add or subtract 1 depending on the sign of (-1)^n. */
    if (n & 1u)      /* n is odd: (-1)^n = -1 -> subtract 1 */
        num = num - (seq_t)1;
    else             /* n is even: (-1)^n = +1 -> add 1      */
        num = num + (seq_t)1;

    /* Final formula: a(n) = num/3 + 1 */
    seq_t result = num / (seq_t)3 + (seq_t)1;
    return result;
}
