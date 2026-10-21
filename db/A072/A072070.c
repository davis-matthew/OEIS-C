/*
 * A072070  Number of integer solutions to the equation 4*x^2 + y^2 + 8*z^2 =
 *          n.
 * https://oeis.org/A072070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of 4*x^2 + y^2 + 8*z^2 = n
 * Range:     u8 n <= 2151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_small.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t nn    = (seq_t)n;

    /* Maximal |x| and |z| that can appear for this n.  We use floor_sqrt_small
     * because the values are tiny for n ≤ 10000. */
    int maxX = (int)floor_sqrt_small(nn / (seq_t)4);
    int maxZ = (int)floor_sqrt_small(nn / (seq_t)8);

    for (int x = -maxX; x <= maxX; ++x) {
        seq_t termX = (seq_t)4 * (seq_t)x * (seq_t)x;
        for (int z = -maxZ; z <= maxZ; ++z) {
            seq_t termZ = (seq_t)8 * (seq_t)z * (seq_t)z;

            /* Skip if the sum of the quadratic parts already exceeds n. */
            if (termX > nn - termZ)
                continue;

            seq_t rem = nn - termX - termZ;               /* y^2 */
            seq_t y   = floor_sqrt_small(rem);
            if (y * y == rem) {                          /* perfect square? */
                if (y == 0)
                    count += (seq_t)1;                   /* y = 0 only once */
                else
                    count += (seq_t)2;                   /* ±y */
            }
        }
    }
    return count;
}
