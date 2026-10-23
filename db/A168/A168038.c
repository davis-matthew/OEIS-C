/*
 * A168038  Squares closest to 2*n.
 * https://oeis.org/A168038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to sqrt(2*n) squared a(n) = A002024(n)^2
 *            The nearest integer is obtained by rounding sqrt(2*n) to the
 *            nearest integer, which is equivalent to setting m =
 *            floor(sqrt(2*n)) and increasing m by one if the distance to the
 *            lower square exceeds the distance to the next square
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 2*n as seq_t */
    seq_t two_n = (seq_t)n + (seq_t)n;

    /* Integer part of sqrt(2*n) */
    seq_t m = floor_sqrt(two_n);

    /* Distance to lower square */
    seq_t diff = two_n - m * m;

    /* If the distance to the lower square is greater than m
       (i.e., > (2*m+1)/2), round up to the next integer. */
    if (diff > m)
        m++;

    return m * m;
}
