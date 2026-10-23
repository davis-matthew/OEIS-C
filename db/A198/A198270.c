/*
 * A198270  Ceiling(n*sqrt(13)).
 * https://oeis.org/A198270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(13))
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 8451, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* ceil(0*sqrt(13)) = 0   */
    seq_t nn = (seq_t)n;
    seq_t x = 13 * nn * nn;     /* x = 13*n^2  */
    seq_t s = floor_sqrt(x);    /* s = floor(sqrt(13*n^2)) = floor(n*sqrt(13)) */
    return s + 1;               /* ceil = floor + 1       */
}
