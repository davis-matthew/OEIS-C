/*
 * A135668  a(n) = ceiling(n + sqrt(n)).
 * https://oeis.org/A135668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n + sqrt(n))
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic with big integers. */
    seq_t xn = (seq_t)n;

    /* Compute the integer floor of the square root of n. */
    seq_t sqrt_floor = floor_sqrt(xn);

    /* Start with the sum n + floor(sqrt(n)). */
    seq_t res = xn + sqrt_floor;

    /* If sqrt(n) is not an integer, add 1 to achieve the ceiling. */
    if (sqrt_floor * sqrt_floor != xn)
        res += 1;

    return res;
}
