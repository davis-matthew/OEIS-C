/*
 * A184304  Number of strings of numbers x(i=1..3) in 0..n with sum i^3*x(i)^2
 *          equal to 27*n^2.
 * https://oeis.org/A184304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts triples (x1,x2,x3) with 0≤xi≤n satisfying 1*x1^2 + 8*x2^2
 *            + 27*x3^2 = 27*n^2
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Integer square‑root of a 64‑bit unsigned integer.
   Returns floor(sqrt(x)).  */
static inline idx_t isqrt_u64(uint64_t x)
{
    double d = sqrt((double)x);
    idx_t r = (idx_t)d;
    /* Adjust for possible rounding errors. */
    while ((uint64_t)(r + 1) * (uint64_t)(r + 1) <= x) ++r;
    while ((uint64_t)r * (uint64_t)r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    /* Use uint64_t for all arithmetic involving n^2 and
       intermediate values to avoid overflow in very small
       seq_t types. */
    uint64_t un = (uint64_t)n;
    uint64_t target = 27ULL * un * un;          /* 27*n^2 */

    seq_t count = 0;                            /* result */

    for (idx_t x3 = 0; x3 <= n; ++x3) {
        uint64_t x3sq = (uint64_t)x3 * (uint64_t)x3;
        uint64_t part3 = 27ULL * x3sq;
        if (part3 > target) break;             /* further x3 only increase */

        uint64_t rem0 = target - part3;        /* 27*n^2 - 27*x3^2 */

        /* For given x3, x2 is bounded by 8*x2^2 <= rem0. */
        uint64_t x2max = isqrt_u64(rem0 / 8ULL);
        if (x2max > n) x2max = n;

        for (idx_t x2 = 0; x2 <= x2max; ++x2) {
            uint64_t x2sq = (uint64_t)x2 * (uint64_t)x2;
            uint64_t part2 = 8ULL * x2sq;
            if (part2 > rem0) break;           /* safety */

            uint64_t rem1 = rem0 - part2;      /* should equal x1^2 */

            /* x1 must satisfy 0 <= x1 <= n and x1^2 == rem1. */
            if (rem1 > un * un) continue;      /* exceeds max square */

            idx_t x1 = isqrt_u64(rem1);
            if ((uint64_t)x1 * (uint64_t)x1 == rem1 && x1 <= n) {
                ++count;
            }
        }
    }

    return count;
}
