/*
 * A070206  Number of obtuse integer triangles with perimeter n having
 *          integral inradius.
 * https://oeis.org/A070206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by brute‑force search over integer sides for perimeter
 *            n. Counts obtuse integer triangles with integral inradius
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   ull_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ull_isqrt.h"

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t count = 0;

    for (uint64_t a = 1; a <= nn / 3; ++a) {
        uint64_t maxb = (nn - a) / 2;
        for (uint64_t b = a; b <= maxb; ++b) {
            uint64_t c = nn - a - b;
            if (a + b <= c) continue;             /* not a triangle */
            if (c * c <= a * a + b * b) continue; /* not obtuse */

            if (nn % 2 == 0) { /* even perimeter, s integer */
                uint64_t s = nn / 2;
                uint64_t y = s - a;
                uint64_t z = s - b;
                uint64_t w = s - c;
                if (y == 0 || z == 0 || w == 0) continue; /* zero area */

                uint64_t area2 = s * y * z * w;            /* <= 6.25e14 */
                uint64_t sqrt_area = ull_isqrt(area2);
                if (sqrt_area * sqrt_area != area2) continue;
                if (sqrt_area % s != 0) continue;          /* r not integer */
            } else { /* odd perimeter, use integer formula */
                uint64_t Q = nn * (nn - 2 * a) * (nn - 2 * b) * (nn - 2 * c);
                if (Q == 0) continue;
                uint64_t sqrtQ = ull_isqrt(Q);
                if (sqrtQ * sqrtQ != Q) continue;
                if (sqrtQ % (2 * nn) != 0) continue;      /* r not integer */
            }

            ++count;
        }
    }

    return (seq_t)count;
}
