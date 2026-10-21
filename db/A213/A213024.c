/*
 * A213024  The number of solutions to x^2 + y^2 + 2*z^2 = n in positive
 *          integers x,y,z.
 * https://oeis.org/A213024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of positive integer solutions (x,y,z) to x^2 + y^2 +
 *            2*z^2 = n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer square root for uint64_t: floor(sqrt(n)) */
static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)  /* mid*mid <= n  */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* external linkage, types supplied by the build */
seq_t a(idx_t n) {
    seq_t count = 0;
    uint64_t zmax = isqrt_u64((uint64_t)n / 2);
    for (uint64_t z = 1; z <= zmax; ++z) {
        /* rem = n - 2*z^2 */
        uint64_t rem = (uint64_t)n - 2 * z * z;
        if (rem == 0) continue;      /* only positive x,y allowed */
        uint64_t xmax = isqrt_u64(rem);
        for (uint64_t x = 1; x <= xmax; ++x) {
            uint64_t y2 = rem - x * x;
            if (y2 == 0) continue;   /* y must be positive */
            uint64_t y = isqrt_u64(y2);
            if (y * y == y2 && y >= 1) {
                count += 1;
            }
        }
    }
    return count;
}
