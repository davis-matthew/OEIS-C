/*
 * A190058  a(n) = n + [n*r/t] + [n*s/t]; r=1/2, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [(n/2)*sin(Pi/3)] + [n*(sin(Pi/3))^2]
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using sin(Pi/3) = sqrt(3)/2, so a(n) = n + floor(n*sqrt(3)/4) + floor(3*n/4) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Promote index to seq_t once to avoid repeated casts */
    seq_t nn = (seq_t)n;

    /* Start with n */
    seq_t res = nn;

    /* Add floor(3*n/4) */
    res += (nn * 3) / 4;

    /* Compute floor(n*sqrt(3)/4)
       sqrt(n*sqrt(3)) = floor_sqrt(3*n^2) gives floor(n*sqrt(3))
       Dividing that by 4 yields floor(n*sqrt(3)/4) */
    seq_t sq = nn * nn;
    sq *= 3;
    seq_t sqrt_part = floor_sqrt(sq) / 4;

    res += sqrt_part;
    return res;
}
