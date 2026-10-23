/*
 * A160324  Number of ways to express n as the sum of a square, a pentagonal
 *          number and a hexagonal number.
 * https://oeis.org/A160324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{<x,y,z> : x,y,z >= 0 and x^2 + (3y^2 - y)/2 + (2z^2 -
 *            z) = n}|
 * Range:     u8 n <= 9981, u16 n <= 9981, u32 n <= 9981, u64 n <= 9981,
 *            u128 n <= 9981, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count all triples (x,y,z) that satisfy the equation.  */
#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 u128;

/* Floor square‑root for unsigned __int128. */
static inline u128 isqrt_u128(u128 v)
{
    u128 lo = 0, hi = ((u128)1 << 64) - 1;
    while (lo <= hi) {
        u128 mid = lo + ((hi - lo) >> 1);
        u128 sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    u128 n128   = (u128)n;
    u128 count  = 0;

    for (idx_t x = 0; ; ++x) {
        u128 xsq = (u128)x * (u128)x;
        if (xsq > n128) break;

        u128 rem1 = n128 - xsq;

        for (idx_t y = 0; ; ++y) {
            u128 tmp   = (u128)3 * y * y - y;   /* 3y^2 - y  */
            u128 pent  = tmp >> 1;              /* (3y^2 - y)/2 */
            if (pent > rem1) break;

            u128 hexrem = rem1 - pent;

            if (hexrem == 0) {
                ++count;                 /* z = 0 */
                continue;
            }

            u128 D = 1 + (u128)8 * hexrem;   /* discriminant 1 + 8*rem */
            u128 s = isqrt_u128(D);
            if (s * s == D && ((s + 1) & 3) == 0)
                ++count;                 /* integer z exists */
        }
    }

    return (seq_t)count;
}
