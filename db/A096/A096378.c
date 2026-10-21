/*
 * A096378  Floor of area of triangle with consecutive integer sides.
 * https://oeis.org/A096378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*sqrt(3*(n+3)*(n-1))/4)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 54, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t ns = (seq_t)n;
    seq_t a0 = ns + 1;                       /* n+1 */
    seq_t d  = 3 * (ns + 3) * (ns - 1);      /* 3*(n+3)*(n-1) */
    seq_t m  = a0 * a0 * d;                  /* (n+1)^2 * d */
    seq_t s  = floor_sqrt(m);                /* floor(sqrt(m))  */
    seq_t q  = s / 4;                        /* floor division by 4 */
    return q;
}
