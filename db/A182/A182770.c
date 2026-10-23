/*
 * A182770  Beatty sequence for 3-sqrt(2).
 * https://oeis.org/A182770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(3-sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                      /* convert index to seq_t   */
    seq_t m  = 2 * nn * nn;                   /* m = 2*n^2                */
    seq_t f  = floor_sqrt(m);                 /* f = floor(n*sqrt(2))     */
    seq_t res = 3 * nn - 1 - f;               /* 3n - 1 - floor(n*sqrt2)  */
    return res;
}
