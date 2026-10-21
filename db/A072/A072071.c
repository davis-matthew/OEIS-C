/*
 * A072071  Number of integer solutions to the equation 4x^2+y^2+32z^2=n.
 * https://oeis.org/A072071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   my_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of integer solutions to 4*x^2 + y^2 + 32*z^2 = n.
 * This is computed by iterating over possible z and, for each, counting
 * integer pairs (x,y) that satisfy 4*x^2 + y^2 = n - 32*z^2.
 */

#include <math.h>
#include "../utils/my_isqrt.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* iterate over z */
    idx_t max_z = my_isqrt(n / 32);
    for (idx_t z = 0; z <= max_z; ++z)
    {
        idx_t z2   = z * z;
        idx_t M    = n - 32 * z2;       /* remaining value after 32*z^2 */
        seq_t  R   = 0;                 /* solutions for fixed M */

        /* iterate over x */
        idx_t max_x = my_isqrt(M / 4);
        for (idx_t x = 0; x <= max_x; ++x)
        {
            idx_t x2 = x * x;
            idx_t D  = M - 4 * x2;     /* required y^2 */

            idx_t y = my_isqrt(D);
            if (y * y == D)
            {
                seq_t wx = (x == 0) ? 1 : 2;  /* ±x, except x=0 */
                seq_t wy = (y == 0) ? 1 : 2;  /* ±y, except y=0 */
                R += wx * wy;
            }
        }

        seq_t wz = (z == 0) ? 1 : 2;      /* ±z, except z=0 */
        total += R * wz;
    }

    return total;
}
