/*
 * A054088  a(n) = A054086(3n); also a bisection of A003511.
 * https://oeis.org/A054088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*sqrt(3)) = floor(n*(1+sqrt(3)))
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* a(0) = 0, though offset starts at 1 */

    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn;              /* (n)^2  */
    t      *= (seq_t)3;             /* 3*n^2 */
    seq_t s  = sqrt_floor(t);        /* floor(sqrt(3*n^2)) = floor(n*sqrt(3)) */
    return nn + s;                   /* a(n) = n + floor(n*sqrt(3)) */
}
