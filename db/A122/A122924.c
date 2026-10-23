/*
 * A122924  Express n as the sum of four squares, x^2+y^2+z^2+w^2,
 *          x>=y>=z>=w>=0, sequentially minimizing the value of x, y and z.
 *          a(n) is that w.
 * https://oeis.org/A122924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = w where n = x^2 + y^2 + z^2 + w^2, x ≥ y ≥ z ≥ w ≥ 0, and
 *            (x, y, z) is chosen lexicographically smallest (first minimize
 *            x, then y, then z)
 * Range:     u8 n <= 63, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t max_x = floor_sqrt(nn);          /* floor(sqrt(n)) */
    /* Try x from 0 upwards; the first feasible x is minimal */
    for (idx_t x = 0; x <= (idx_t)max_x; ++x)
    {
        seq_t X2 = (seq_t)x * (seq_t)x;
        if (X2 > nn) break;               /* further x will only increase X2 */

        for (idx_t y = 0; y <= x; ++y)
        {
            seq_t Y2 = X2 + (seq_t)y * (seq_t)y;
            if (Y2 > nn) break;           /* further y will only increase Y2 */

            seq_t r12 = nn - Y2;          /* remainder to be split into z^2 + w^2 */
            seq_t max_z = floor_sqrt(r12);
            if (max_z > y) max_z = (seq_t)y;   /* respect ordering y ≥ z */

            for (idx_t z = 0; z <= (idx_t)max_z; ++z)
            {
                seq_t Z2 = Y2 + (seq_t)z * (seq_t)z;
                if (Z2 > nn) break;       /* further z will only increase Z2 */

                seq_t remaining = nn - Z2;        /* w^2 must equal this */
                seq_t w = floor_sqrt(remaining);
                if (w * w == remaining && w <= z)
                    return (seq_t)w;          /* minimal w found for minimal (x,y,z) */
            }
        }
    }
    /* For n ≥ 0 a representation always exists; return 0 as a safe default */
    return (seq_t)0;
}
