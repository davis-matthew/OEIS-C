/*
 * A128018  Expansion of (1-4*x)/(1-2*x+4*x^2).
 * https://oeis.org/A128018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^(n+1)*cos(pi*(n+1)/3) (generated from the recurrence
 *            a(0)=1, a(1)=-2 and a(n)=2*a(n-1)-4*a(n-2) ) Computation uses
 *            only integer arithmetic and the utility pow2() for powers of two
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"          /* provides seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t p = pow2(n);

    /* (n+1) mod 6 determines the sign and possible extra factor of 2 */
    idx_t r = (n + 1) % 6;

    switch (r)
    {
        case 0:          /* 2^(n+1) */
            return (seq_t)2 * p;
        case 1:          /* 2^n   */
            return p;
        case 2:          /* -2^n  */
            return -(p);
        case 3:          /* -2^(n+1) */
            return -(seq_t)2 * p;
        case 4:          /* -2^n  */
            return -(p);
        case 5:          /* 2^n   */
            return p;
        default:         /* unreachable */
            return 0;
    }
}
