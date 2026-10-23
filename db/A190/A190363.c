/*
 * A190363  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sqrt(5/4), t=sqrt(4/5).
 * https://oeis.org/A190363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + [n*sqrt(5/4)] + [n/4]
 * Range:     u8 n <= 7, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* integer value of floor(n * sqrt(5/4)):
       floor_sqrt( floor(5 * n^2 / 4) ) gives the exact floor of the irrational product. */
    seq_t m = sn * sn * (seq_t)5 / 4;
    seq_t termA = floor_sqrt(m);
    seq_t termB = sn / 4;
    return sn * 2 + termA + termB;
}
