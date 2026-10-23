/*
 * A198395  a(n) = ceiling(n*sqrt(17)).
 * https://oeis.org/A198395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(17))
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 7280, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t m = nn * nn * 17;          /* m = 17*n^2 */
    seq_t f = floor_sqrt(m);         /* floor(sqrt(17*n^2)) = floor(n*sqrt(17)) */
    return f + 1;                    /* ceil(n*sqrt(17)) = floor + 1 for n>0 */
}
