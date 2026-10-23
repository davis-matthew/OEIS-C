/*
 * A029594  Theta series of quadratic form with Gram matrix [ 1, 0, 0; 0, 2,
 *          0; 0, 0, 3 ].
 * https://oeis.org/A029594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y,z) ∈ Z³ : x² + 2·y² + 3·z² = n }
 * Range:     u8 n <= 875, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor square root of a 64‑bit unsigned integer */
static inline idx_t isqrt(idx_t v) {
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 == v) return mid;
        if (m2 < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t z = 0; ; ++z) {
        unsigned __int128 z2 = (unsigned __int128)z * z;
        unsigned __int128 t3 = 3 * z2;
        if (t3 > n) break;
        idx_t rem_z = n - (idx_t)t3;
        int count_z = (z == 0) ? 1 : 2;
        for (idx_t y = 0; ; ++y) {
            unsigned __int128 y2 = (unsigned __int128)y * y;
            unsigned __int128 t2 = 2 * y2;
            if (t2 > rem_z) break;
            idx_t rem = rem_z - (idx_t)t2;
            idx_t s = isqrt(rem);
            if ((unsigned __int128)s * s == rem) {
                int count_y = (y == 0) ? 1 : 2;
                seq_t add = (seq_t)count_z * (seq_t)count_y;
                if (rem == 0)
                    total += add;
                else
                    total += add * 2;
            }
        }
    }
    return total;
}
