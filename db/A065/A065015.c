/*
 * A065015  Sum divides product: number of integer solutions (w,x,y,z), w >= x
 *          >= y >= z > 0, to the equation w*x*y*z = n*(w+x+y+z).
 * https://oeis.org/A065015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   _floor_cuberoot, _floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A065015 – Sum divides product
 *  a(n) = # of integer solutions w ≥ x ≥ y ≥ z > 0 to w*x*y*z = n*(w+x+y+z)
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_cuberoot.h"
#include "../utils/_floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety, offset starts at 1 */

    const idx_t limitXYZ = 4 * n;              /* xyz <= 4n (proved from wxyz <= 4nw) */

    seq_t count = 0;

    idx_t zmax = _floor_cuberoot(limitXYZ);
    for (idx_t z = 1; z <= zmax; ++z)
    {
        /* y must satisfy y^2 <= limitXYZ / z  (since x≥y and xy*z <= limitXYZ) */
        idx_t ymax = _floor_sqrt(limitXYZ / z);
        for (idx_t y = z; y <= ymax; ++y)
        {
            idx_t yz = y * z;
            /* x must satisfy xy*z <= limitXYZ  →  x <= limitXYZ / yz  */
            idx_t xmax = limitXYZ / yz;
            for (idx_t x = y; x <= xmax; ++x)
            {
                /* xyz must be > n to keep denominator positive */
                uint64_t xyz = (uint64_t)x * yz;
                if (xyz <= n) continue;

                seq_t denom = (seq_t)xyz - (seq_t)n;            /* > 0 */
                seq_t numerator = (seq_t)n * ((seq_t)x + (seq_t)y + (seq_t)z);

                if (numerator % denom != 0) continue;           /* z not integer */

                seq_t w = numerator / denom;                    /* computed w */

                if (w < (seq_t)x) continue;                    /* enforce w ≥ x */

                /* ordering w ≥ x ≥ y ≥ z already guaranteed by loops */
                ++count;
            }
        }
    }
    return count;
}
