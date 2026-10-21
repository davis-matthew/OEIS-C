/*
 * A070141  Number of obtuse integer triangles with perimeter n having
 *          integral area.
 * https://oeis.org/A070141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of obtuse integer triangles with perimeter n
 *            having integral area
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_perfect_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/is_perfect_square.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t a = 1; a <= n/3; ++a) {
        for (idx_t b = a; b <= (n - a)/2; ++b) {
            idx_t c = n - a - b;
            if (c < b) continue;                     /* ensure a <= b <= c */
            if (a + b <= c) continue;                /* triangle inequality */
            if ((__int128)c*c <= (__int128)a*a + (__int128)b*b) continue; /* obtuse test */

            __int128 aa = a, bb = b, cc = c;
            __int128 T = 2*aa*aa*bb*bb + 2*aa*aa*cc*cc + 2*bb*bb*cc*cc
                       - aa*aa*aa*aa - bb*bb*bb*bb - cc*cc*cc*cc;
            if (T % 16 != 0) continue;
            __int128 area_sq = T / 16;               /* area^2 */
            if (!is_perfect_square((unsigned long long)area_sq)) continue;

            ++count;
        }
    }
    return count;
}
