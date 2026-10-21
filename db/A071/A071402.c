/*
 * A071402  Rounded volume of a regular icosahedron with edge length n.
 * https://oeis.org/A071402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( (15*n^3 + floor_sqrt(125*n^6) + 6) / 12 )
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 734, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Integer square root for 128‑bit values (used only when seq_t is ≤8 bytes) */
static uint64_t isqrt128(__int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)-1;          /* max 64‑bit value */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            if (mid == (uint64_t)-1) break;      /* avoid overflow on next step */
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;                                 /* floor(√x) */
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;            /* convert index to seq_t */
    seq_t n3 = N * N * N;          /* n^3 */

    /* Small build (seq_t <= 8 bytes) – use 128‑bit arithmetic to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        __int128 nn3 = (__int128)n3;
        __int128 n6 = nn3 * nn3;                       /* n^6  (fits in 128 bits) */
        __int128 B  = (__int128)125 * n6;              /* 125*n^6 */
        uint64_t sqrtB = isqrt128(B);                  /* floor(sqrt(B)) */

        /* arithmetic fits in 64 bits for the range of n typically used */
        seq_t num = (seq_t)((__int128)15 * n3 + sqrtB + 6);
        return num / (seq_t)12;
    }

    /* Big‑int build – use seq_t arithmetic and the provided floor_sqrt() */
    seq_t n6 = n3 * n3;                /* n^6 */
    seq_t B  = n6 * (seq_t)125;        /* 125*n^6 */
    seq_t sqrtB = floor_sqrt(B);       /* floor(sqrt(B)) */

    seq_t num = (seq_t)15 * n3 + sqrtB + (seq_t)6;
    return num / (seq_t)12;
}
