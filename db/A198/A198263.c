/*
 * A198263  a(n) = ceiling(n*sqrt(8)).
 * https://oeis.org/A198263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*sqrt(8))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* m = 2*n */
    seq_t m = (seq_t)n * 2;

    /* Compute t = 2*m*m */
    seq_t t = m * m;
    t <<= 1;          /* t = 2 * (m*m) */

    /* floor_sqrt(t) gives floor(m*sqrt(2)) = floor(2*n*sqrt(2)) */
    seq_t s = floor_sqrt(t);

    return s + 1;
}
