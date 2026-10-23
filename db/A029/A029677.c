/*
 * A029677  Theta series of quadratic form with Gram matrix [ 4, 1, 0; 1, 4,
 *          2; 0, 2, 6 ].
 * https://oeis.org/A029677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) such that * 2*x^2 + x*y
 *            + 2*y^2 + 2*y*z + 3*z^2 = n, with n ≥ 0 (offset 0)
 * Range:     u8 n <= 2042, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return 1u;               /* only the zero triple */

    /* Upper bound for |x| and |y|.  For any solution we have
       2|x|^2 + |x||y| + 2|y|^2 ≤ n, so |x|,|y| ≤ sqrt(2n)+1. */
    int64_t bound = (int64_t)(sqrt((double)n * 2.0) + 1.0);

    seq_t total = 0;

    for (int64_t ix = -bound; ix <= bound; ++ix)
    {
        for (int64_t iy = -bound; iy <= bound; ++iy)
        {
            /* part = 2x^2 + xy + 2y^2 */
            int64_t part = 2*ix*ix + ix*iy + 2*iy*iy;
            int64_t rhs  = (int64_t)n - part;       /* n - part */

            /* Discriminant of 3z^2 + 2yz + (part - n) = 0:
               D = (2y)^2 - 4*3*(part - n) = 4y^2 + 12*rhs. */
            int64_t D = 4*iy*iy + 12*rhs;
            if (D < 0) continue;

            /* Integer square root of D (use double, correct by +-1). */
            int64_t sqrtD = (int64_t)(sqrt((double)D) + 0.5);
            if (sqrtD*sqrtD != D) continue;

            /* First root: z = (-2y + sqrtD) / 6. */
            int64_t num1 = -2*iy + sqrtD;
            if (num1 % 6 == 0)
                ++total;

            /* Second root exists only if sqrtD != 0. */
            if (sqrtD != 0)
            {
                int64_t num2 = -2*iy - sqrtD;
                if (num2 % 6 == 0)
                    ++total;
            }
        }
    }

    return total;
}
