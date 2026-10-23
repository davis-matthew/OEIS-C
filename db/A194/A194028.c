/*
 * A194028  Beatty sequence for sqrt(12).
 * https://oeis.org/A194028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(12))
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 8580, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (nn * nn) * 12;   /* compute 12*n^2 */
    return floor_sqrt(val);
}
