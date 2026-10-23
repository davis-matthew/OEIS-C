/*
 * A171984  Beatty sequence for sqrt(17).
 * https://oeis.org/A171984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * sqrt(17))
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute floor(sqrt(17*n^2)) which equals floor(n*sqrt(17)) */
    seq_t nn = (seq_t)n * (seq_t)n;    /* n^2 */
    seq_t t  = nn * (seq_t)17;         /* 17*n^2 */
    return floor_sqrt(t);
}
