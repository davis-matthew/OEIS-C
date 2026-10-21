/*
 * A229098  Smallest area A of Heron triangles with sides (a, b, c) in
 *          arithmetic progression of the form b - d(n), b, b + d(n), where
 *          d(n) = A091998(n) = 12*n +- 1.
 * https://oeis.org/A229098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest area of a Heron triangle with sides (b-d(n), b,
 *            b+d(n)), where d(n) = 1 for n=1, and for n>1: if n is even then
 *            d(n)=12*(n/2)-1, else d(n)=12*(n/2)+1
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   d_of_n, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/d_of_n.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* out of range, return 0 */
        return 0;

    seq_t d  = d_of_n(n);      /* d(n) */
    seq_t start = 2 * d + 1;   /* minimal b satisfying b > 2d */

    for (seq_t b = start;; ++b)
    {
        seq_t t = b * b - 4 * d * d;   /* b^2 - 4 d^2 */

        if (t % 3 != 0)               /* must be divisible by 3 */
            continue;

        seq_t v2 = t / 3;             /* (b^2 - 4 d^2)/3 */
        seq_t v = floor_sqrt(v2);     /* integer square root */

        if (v * v != v2)              /* must be a perfect square */
            continue;

        if ((b * v) % 4 != 0)         /* area must be integer */
            continue;

        seq_t area = (3 * b * v) / 4; /* (3·b·v)/4, division exact */
        return area;
    }
}
