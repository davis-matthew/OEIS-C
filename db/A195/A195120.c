/*
 * A195120  a(n) = 2*n - floor(n*sqrt(3)).
 * https://oeis.org/A195120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - floor(n*sqrt(3))
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* cast index to seq_t */
    seq_t t  = nn * nn;                /* n^2 */
    t      = t * (seq_t)3;             /* 3*n^2 */
    seq_t s = floor_sqrt(t);           /* floor(sqrt(3*n^2)) = floor(n*sqrt(3)) */
    seq_t res = ((seq_t)2) * nn - s;   /* 2*n - s */
    return res;
}
