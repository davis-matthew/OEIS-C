/*
 * A195119  a(n) = 2*n - floor(n*sqrt(2)).
 * https://oeis.org/A195119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - floor(n*sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* floor(n*sqrt(2)) equals floor_sqrt(2*n*n) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = (seq_t)2 * nn * nn;   /* 2*n^2 */
    seq_t r = floor_sqrt(prod);        /* floor(n*sqrt(2)) */
    return (seq_t)2 * nn - r;          /* 2*n - floor(n*sqrt(2)) */
}
