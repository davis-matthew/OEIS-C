/*
 * A063826  Let 1, 2, 3, 4 represent moves to the right, down, left and up;
 *          this sequence describes the movements in the clockwise square
 *          spiral (a.k.a. Ulam Spiral).
 * https://oeis.org/A063826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = floor(sqrt(4*n + 1)) mod 4, with 0 mapped to 4 (so 1 ≤
 *            a(n) ≤ 4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(x)).  Uses binary search. */
static inline unsigned long long sqrt_uint128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = (1ULL << 33);        /* safe upper bound */
    unsigned long long ans = 0;
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    /* m = 4*n + 1 (computed in 128 bits to avoid overflow) */
    unsigned __int128 m = (unsigned __int128)n * 4 + 1;

    /* floor sqrt of m */
    unsigned long long s = sqrt_uint128(m);

    /* result modulo 4, mapping 0 → 4 */
    unsigned int r = (unsigned int)(s & 3ULL);
    return r == 0 ? (seq_t)4 : (seq_t)r;
}
