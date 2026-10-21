/*
 * A067753  Number of primitive solutions in nonnegative integers of
 *          x*y+x*z+y*z = n.
 * https://oeis.org/A067753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primitive ordered triples (x,y,z) (x,y,z≥0)
 *            with xy + xz + yz = n (primitive means gcd(x,y,z) = 1)
 * Range:     u8 n <= 1318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd64.h"

/* helper for greatest common divisor on 64-bit integers */

seq_t a(idx_t n)
{
    /* n > 0 as per OEIS offset 1 */
    seq_t count = 0;
    for (idx_t x = 0; x <= n; ++x) {
        for (idx_t y = 0; y <= n; ++y) {
            if (x == 0 && y == 0) continue;      /* xy=0, but x+y=0 => undefined */
            __uint128_t prod = (__uint128_t)x * y;
            if (prod > n) break;                 /* xy > n ⇒ rem negative */
            __uint128_t denom = ( __uint128_t)(x + y);
            if (denom == 0) continue;            /* should not happen, guard */

            __uint128_t rem = (__uint128_t)n - prod;   /* n - xy ≥ 0 */
            if (rem % denom != 0) continue;          /* z must be integer */

            __uint128_t z128 = rem / denom;
            if (z128 > (__uint128_t)UINT64_MAX) continue;   /* safety, shouldn't happen */

            uint64_t z = (uint64_t)z128;
            uint64_t g = gcd64(gcd64(x, y), z);
            if (g == 1) ++count;
        }
    }
    return count;
}
