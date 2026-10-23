/*
 * A184626  floor(nr+h), where r=sqrt(3), h=1/4; complement of A184627.
 * https://oeis.org/A184626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3) + 1/4)
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /*  n >= 1  */
    /* Compute y = 3*n^2  */
    seq_t y = (seq_t)3 * n * n;

    /* t = floor(sqrt(y)) = floor(n*sqrt(3))  */
    seq_t t = floor_sqrt(y);

    /* Determine whether the fractional part of n*sqrt(3) is ≥ 3/4.
       Inequality derived from (t + 3/4)^2 <= 3*n^2,
       i.e. 48*n^2 >= 16*t^2 + 24*t + 9.  */
    seq_t left  = (seq_t)48 * n * n;
    seq_t right = (seq_t)16 * t * t + (seq_t)24 * t + (seq_t)9;

    return (left >= right) ? t + 1 : t;
}
