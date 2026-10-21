/*
 * A235204  Number of integer lattice points inside the square ABCD with side
 *          length n>0 with A(0|0), B(n*sqrt(2)/2| n*sqrt(2)/2), C(0|
 *          n*sqrt(2)) and D(-n*sqrt(2)/2| n*sqrt(2)/2).
 * https://oeis.org/A235204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*z^2 + 1 - (-1)^z)/4 where z = ceil(sqrt(2)*n)
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* target = 2*n^2 */
    seq_t target = (seq_t)n * (seq_t)n * (seq_t)2;

    /* compute floor of sqrt(target) */
    seq_t f = floor_sqrt(target);
    seq_t f2 = f * f;

    /* ceil sqrt = f if exact, otherwise f+1 */
    seq_t z = (f2 == target) ? f : f + (seq_t)1;

    /* a = (z^2 + (z & 1)) / 2  -- handles even/odd cases */
    seq_t z2 = z * z;
    return (z2 + (z & (seq_t)1)) >> 1;
}
