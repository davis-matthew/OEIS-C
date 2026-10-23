/*
 * A184113  n + floor(4*sqrt(n+1)).
 * https://oeis.org/A184113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(4*sqrt(n+1))
 * Range:     u8 n <= 2, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute floor_sqrt(16*(n+1)) which equals floor(4*sqrt(n+1)) */
    seq_t np1 = (seq_t)n + 1;
    seq_t big  = np1 * (seq_t)16;
    seq_t s    = floor_sqrt(big);
    return (seq_t)n + s;
}
