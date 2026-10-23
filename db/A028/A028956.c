/*
 * A028956  Theta series of quadratic form (or lattice) with Gram matrix [ 4,
 *          1; 1, 4 ].
 * https://oeis.org/A028956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integer solutions (x,y) to 2*x^2 + x*y + 2*y^2 = n
 * Range:     u8 n <= 7, u16 n <= 1631, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* rhs = 8*n */
    seq_t rhs = (seq_t)8 * (seq_t)n;

    /* bound on |x|: 15*x^2 <= rhs   =>   x^2 <= rhs/15   =>   |x| <= floor_sqrt(rhs/15) */
    seq_t bound = rhs / (seq_t)15;
    seq_t sqb   = floor_sqrt(bound);
    long long maxx = (long long)sqb;          /* safe for the intended ranges */

    seq_t count = 0;
    for (long long x = -maxx; x <= maxx; ++x) {
        /* x^2 as unsigned */
        seq_t ax = (seq_t)(x >= 0 ? x : -x);
        seq_t x_sq = ax * ax;

        /* delta = rhs - 15*x^2   (must be >= 0) */
        seq_t lhs = (seq_t)15 * x_sq;
        if (lhs > rhs) continue;              /* safety guard */
        seq_t delta = rhs - lhs;

        seq_t s = floor_sqrt(delta);
        if (s * s != delta) continue;         /* delta not a perfect square */

        if (s == 0) {
            /* only one candidate y: (-x)/4 */
            if ((-x) % 4 == 0) count += 1;
        } else {
            /* two candidates: (-x ± s)/4 */
            long long ls = (long long)s;
            for (int sign = -1; sign <= 1; sign += 2) {
                long long num = -x + sign * ls;
                if (num % 4 == 0) count += 1;
            }
        }
    }
    return count;
}
