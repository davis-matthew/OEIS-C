/*
 * A184924  n+[sn/r]+[tn/r]+[un/r], where []=floor and r=1, s=sqrt(3),
 *          t=sqrt(5), u=sqrt(7).
 * https://oeis.org/A184924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)) + floor(n*sqrt(5)) +
 *            floor(n*sqrt(7))
 * Range:     u8 n <= 33, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt_mn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_mn.h"

/* Return floor(sqrt(x)) for a 128‑bit unsigned integer using binary search. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = (x + 1) >> 1;
    if (hi < 2) hi = 2;                       /* guard against tiny x */
    /* Ensure hi^2 > x.  For small x the above is enough. */
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t r3 = floor_sqrt_mn(n, 3);
    seq_t r5 = floor_sqrt_mn(n, 5);
    seq_t r7 = floor_sqrt_mn(n, 7);
    return nn + r3 + r5 + r7;
}
