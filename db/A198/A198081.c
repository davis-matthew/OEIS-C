/*
 * A198081  a(n) = ceiling(n*sqrt(3)).
 * https://oeis.org/A198081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(3))
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t s  = (seq_t)3 * nn * nn;   /* s = 3 * n^2 */
    seq_t f  = floor_sqrt(s);         /* f = floor(n*sqrt(3)) */
    return f + 1;                     /* ceil(n*sqrt(3)) = floor + 1 for n>0 */
}
