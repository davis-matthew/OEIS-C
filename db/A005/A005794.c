/*
 * A005794  Number of SO_1^{2+}(Z) orbits of Lorentzian modular group.
 * https://oeis.org/A005794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Count orbit representatives (z, x, y) of n = z^2 - x^2 -
 *            y^2 with z > x >= 0, z > y >= 0, z >= x+y. Boundary points are
 *            identified: (z, 0, y) ~ (z, y, 0), and on z = x+y, (z, x, y) ~
 *            (z, y, x) (keep x >= y). Points with y = 0 come from the
 *            factorizations n = (z-x)(z+x); points with x, y >...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   a5794_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a5794_isqrt.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t cnt = 0;
    uint64_t d, x, y;

    /* y = 0: n = (z-x)(z+x), d = z-x <= e = z+x, same parity */
    for (d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            uint64_t e = N / d;
            if (((d + e) & 1) == 0) cnt++;
        }
    }

    /* x, y >= 1 with 2xy <= n */
    for (y = 1; 2 * y <= N; y++) {
        for (x = 1; 2 * x * y <= N; x++) {
            uint64_t s = N + x * x + y * y;
            uint64_t z = a5794_isqrt(s);
            if (z * z != s) continue;
            if (z < x + y) continue;
            if (z == x + y && x < y) continue;
            cnt++;
        }
    }
    return (seq_t)cnt;
}
