/*
 * A066360  Number of unordered solutions in positive integers of xy + xz + yz
 *          = n with gcd(x,y,z) = 1.
 * https://oeis.org/A066360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = #{(x,y,z)∈N³ | xy + xz + yz = n, gcd(x,y,z)=1, and x ≤ y
 *            ≤ z}
 * Range:     u8 n <= 563, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t count = 0;

    for (idx_t x = 1; ; ++x) {
        seq_t xx = (seq_t)x;
        /* If 3*x^2 > n then no further solutions. */
        if (xx * xx * 3 > N) break;

        for (idx_t y = x; ; ++y) {
            seq_t yy = (seq_t)y;
            seq_t xy = xx * yy;

            /* Stop inner loop if xy already exceeds n. */
            if (xy > N) break;

            seq_t numerator = N - xy;          /* safe: xy ≤ N */
            seq_t denom = xx + yy;             /* denom ≥ 2 */

            if (numerator % denom != 0) continue;
            seq_t z = numerator / denom;
            if (z < yy) continue;              /* keep x ≤ y ≤ z */

            seq_t g = gcd(gcd(xx, yy), z);
            if (g == 1) ++count;
        }
    }

    return count;
}
