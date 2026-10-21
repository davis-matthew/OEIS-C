/*
 * A072069  Number of integer solutions to the equation 2x^2+y^2+32z^2=m for
 *          an odd number m=2n-1.
 * https://oeis.org/A072069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y,z) to 2*x^2 + y^2 +
 *            32*z^2 = 2*n-1 (n ≥ 1)
 * Range:     u8 n <= 2780, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/integer_sqrt.h"

seq_t a(idx_t n)
{
    /* m = 2*n - 1 is the odd number being represented. */
    idx_t m = n * (idx_t)2 - (idx_t)1;

    uint64_t total = 0;

    /* Enumerate possible z (both signs handled multiplicatively). */
    for (idx_t z = 0; (uint64_t)32 * z * z <= m; ++z) {
        uint64_t mult_z = (z == 0) ? 1 : 2;          /* z = ±z, z = 0 counted once */
        idx_t remz = m - (idx_t)32 * z * z;          /* remaining after 32*z^2 */

        /* Enumerate possible x (both signs handled similarly). */
        for (idx_t x = 0; (uint64_t)2 * x * x <= remz; ++x) {
            uint64_t mult_x = (x == 0) ? 1 : 2;      /* x = ±x, x = 0 counted once */
            idx_t rem = remz - (idx_t)2 * x * x;    /* remaining after 2*x^2 */

            /* Check whether rem is a perfect square (y^2). */
            uint64_t y = integer_sqrt((uint64_t)rem);
            if (y * y == (uint64_t)rem) {
                uint64_t mult_y = (y == 0) ? 1 : 2;  /* y = ±y, y = 0 counted once */
                total += mult_z * mult_x * mult_y;
            }
        }
    }

    return (seq_t)total;          /* cast to the sequence type */
}
