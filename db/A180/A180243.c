/*
 * A180243  a(n) = number of ways in which the square of n can be expressed as
 *          the sum of a square, a cube and a fourth power.
 * https://oeis.org/A180243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of positive integers such that
 *            x^2 + y^3 + z^4 = n^2
 * Range:     u8 n <= 8, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9504 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    /* n must be at least 1 according to the OEIS offset. */
    if (n < 1) return 0;
    seq_t count = 0;

    /* Square of n. */
    seq_t m = (seq_t)n * (seq_t)n;

    /* Iterate over all possible z (the base of the fourth power). */
    for (idx_t z = 1;; ++z) {
        seq_t z4 = (seq_t)z * z;          /* z^2 */
        z4 = z4 * z4;                     /* z^4 */
        if (z4 > m) break;                /* no larger z needed */

        seq_t rem1 = m - z4;              /* remaining after z^4 */
        if (rem1 <= 0) continue;          /* should not happen due to break */

        /* Iterate over all possible y (the base of the cube). */
        for (idx_t y = 1;; ++y) {
            seq_t y3 = (seq_t)y * y * y;  /* y^3 */
            if (y3 > rem1) break;         /* no larger y will fit */

            seq_t rem2 = rem1 - y3;       /* remaining after y^3 */
            if (rem2 <= 0) continue;      /* should not happen */

            seq_t x = floor_sqrt(rem2);   /* floor(sqrt(rem2)) */
            if (x > 0 && (x * x) == rem2) {
                /* x is a positive integer and x^2 == rem2 */
                count++;
            }
        }
    }

    return count;
}
