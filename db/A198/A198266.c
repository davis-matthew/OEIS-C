/*
 * A198266  a(n) = ceiling(n*sqrt(11)).
 * https://oeis.org/A198266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(11))
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 9402, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* a(0) = 0 by definition */

    seq_t nn = (seq_t)n;                 /* cast index to seq_t */
    /* v = 11 * n^2  (no overflow for the ranges of interest) */
    seq_t v = 11 * (nn * nn);
    /* floor_sqrt returns floor(sqrt(v)) = floor(n*sqrt(11)) */
    seq_t s = floor_sqrt(v);
    return s + 1;                        /* ceil(n*sqrt(11)) = floor + 1 */
}
