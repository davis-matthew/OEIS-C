/*
 * A171975  Integer part of the circumsphere radius of a regular tetrahedron
 *          with edge length n.
 * https://oeis.org/A171975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(6)/4)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Offset is 1, so a(1) corresponds to n = 1. */
    if (n == 0) return 0;          /* Just a safety guard, not expected (offset is 1). */
    seq_t sn = (seq_t)n;
    seq_t m = sn * sn;              /* Compute n^2. */
    m *= 6;                         /* Compute 6*n^2. */
    seq_t s = floor_sqrt(m);        /* floor(n * sqrt(6)). */
    return s / 4;                   /* integer division gives floor(n*sqrt(6)/4). */
}
