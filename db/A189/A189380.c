/*
 * A189380  a(n) = n + floor(n*s/r) + floor(n*t/r); r=1, s=-1+sqrt(2),
 *          t=1+sqrt(2).
 * https://oeis.org/A189380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2*floor_sqrt(2*n*n)
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from a(n) = n + floor(n*(-1+sqrt(2))) + floor(n*(1+sqrt(2))) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 2*n*n safely in seq_t */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t two_nn = (seq_t)2 * nn;

    /* floor_sqrt gives floor(sqrt(x)) */
    seq_t sq = floor_sqrt(two_nn);

    /* a(n) = n + 2*floor_sqrt(2*n*n) */
    return (seq_t)n + (seq_t)2 * sq;
}
