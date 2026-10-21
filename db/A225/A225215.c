/*
 * A225215  Floor of the Euclidean distance of a point on the (1, 1, 1; 1, 1,
 *          1) 3D walk.
 * https://oeis.org/A225215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt( floor((n+2)/3)^2 + floor((n+1)/3)^2 +
 *            floor(n/3)^2 ))
 * Range:     u8 n <= 13, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Return 0 for n==0; sequence is defined for n >= 1 */
    if (n == 0) return 0;

    /* Compute the coordinates after n steps */
    idx_t x = (n + 2) / 3;
    idx_t y = (n + 1) / 3;
    idx_t z = n / 3;

    /* Convert to seq_t and compute the squared distance */
    seq_t sx = (seq_t)x;
    seq_t sy = (seq_t)y;
    seq_t sz = (seq_t)z;
    seq_t d  = sx * sx + sy * sy + sz * sz;

    /* Return the floor of the Euclidean distance */
    return floor_sqrt(d);
}
