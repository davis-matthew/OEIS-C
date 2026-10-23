/*
 * A177102  Beatty sequence for sqrt(10).
 * https://oeis.org/A177102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * sqrt(10))
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute floor( sqrt(10 * n^2) ) which equals floor(n*sqrt(10)) */
    seq_t sq = (seq_t)n * (seq_t)n;     /* n^2  */
    sq = sq * (seq_t)10;                /* 10*n^2 */
    return floor_sqrt(sq);
}
