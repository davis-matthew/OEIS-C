/*
 * A140991  a(n) = (1/9)*(7*2^n + (-1)^n*(3*n+2)) - (n-1)^2.
 * https://oeis.org/A140991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/9)*(7*2^n + (-1)^n*(3*n+2)) - (n-1)^2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function handles all n >= 0, uses the provided pow2() function
   for arbitrary-precision powers of two, and performs the arithmetic
   in the seq_t type.                                             */

#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return (seq_t)0;

    /* compute 2^n as seq_t */
    seq_t pow2_n = pow2(n);

    /* termA = 7 * 2^n */
    seq_t termA = (seq_t)7 * pow2_n;

    /* small = 3*n + 2 fits comfortably in idx_t */
    idx_t small = (idx_t)3 * n + (idx_t)2;
    seq_t termB = (seq_t)small;

    /* compute numerator = 7*2^n + (-1)^n*(3*n+2) */
    seq_t sum;
    if (n & 1)   /* n odd: subtract small */
        sum = termA - termB;
    else         /* n even: add small   */
        sum = termA + termB;

    /* divide by 9 (exact division, result is integer) */
    seq_t div9 = sum / (seq_t)9;

    /* subtract (n-1)^2  */
    seq_t sub = (seq_t)(n - 1) * (seq_t)(n - 1);

    return div9 - sub;
}
