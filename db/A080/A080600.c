/*
 * A080600  a(n) = ceiling(n*(3 + sqrt(13))/2).
 * https://oeis.org/A080600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*(3 + sqrt(13))/2)
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 8947, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (!n)
        return 0; /* a(0) = 0 */

    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t s  = floor_sqrt( ((seq_t)13) * nn * nn ); /* floor(n*sqrt13) */
    seq_t p  = 3 * nn + s;              /* 3n + floor(n*sqrt13) */
    seq_t res = p / 2 + 1;              /* floor(p/2) + 1 equals ceil(n*(3+sqrt13)/2) */

    return res;
}
