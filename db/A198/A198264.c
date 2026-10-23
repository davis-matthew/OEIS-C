/*
 * A198264  Round(n*sqrt(10)).
 * https://oeis.org/A198264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(10))
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* m = 10 * n^2 */
    seq_t nn = (seq_t)n;
    seq_t m  = nn * nn * (seq_t)10;

    /* k = floor(sqrt(m)) */
    seq_t k = floor_sqrt(m);

    /* Compute k^2 and the threshold k^2 + k + 1 */
    seq_t k2       = k * k;
    seq_t threshold = k2 + k + (seq_t)1;

    /* Decide whether to round up */
    seq_t res = k + (seq_t)(m >= threshold);
    return res;
}
