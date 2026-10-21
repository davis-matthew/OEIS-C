/*
 * A080817  Leading diagonal of triangle in A080818.
 * https://oeis.org/A080817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (ceil(sqrt(n*(n+1)/2)))^2 - n*(n-1)/2
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   _isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function _isqrt to compute the floor square root 
   of an arbitrary‑precision integer. */

#include "../utils/_isqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the result type */
    seq_t ns = (seq_t)n;

    /* Compute the nth triangular number T_n = n*(n+1)/2 */
    seq_t tN = ns * (ns + 1) / 2;

    /* Floor square root of T_n */
    seq_t floor_sqrt = _isqrt(tN);

    /* Ceil square root: increment if floor^2 < T_n */
    seq_t ceil_sqrt = floor_sqrt;
    if (floor_sqrt * floor_sqrt < tN)
        ceil_sqrt = floor_sqrt + 1;

    /* Square of the ceil square root */
    seq_t ceil_sq = ceil_sqrt * ceil_sqrt;

    /* Compute T_{n-1} = n*(n-1)/2 */
    seq_t tNm1 = ns * (ns - 1) / 2;

    /* Final formula */
    return ceil_sq - tNm1;
}
