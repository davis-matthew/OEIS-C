/*
 * A190348  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(5/3), t=sqrt(3/5).
 * https://oeis.org/A190348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3/5)) + floor(3*n/5)
 * Range:     u8 n <= 10, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Intermediate arithmetic is done in idx_t to avoid overflow for
       large n; the final value is then converted to seq_t. */
    idx_t nn = n;
    idx_t t3  = (nn * 3) / 5;          /* floor(3*n/5)        */
    idx_t tri = (nn * nn * 3) / 5;     /* floor((3*n^2)/5)   */

    seq_t sroot = floor_sqrt((seq_t)tri);  /* floor(n*sqrt(3/5)) */
    seq_t sn     = (seq_t)nn;
    seq_t s_t3   = (seq_t)t3;

    return sn + sroot + s_t3;
}
