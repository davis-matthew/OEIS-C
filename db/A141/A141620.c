/*
 * A141620  First differences of A120070.
 * https://oeis.org/A141620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) =
 *            ((A^2+3*A-2*n)*(-A^2+A+2*(n+4))-(B^2+3*B-2*(n-1))*(-B^2+B+2*(n+3)))/4,
 *            where A = floor((sqrt(8*n+9)-1)/2) and B =
 *            floor((sqrt(8*n+1)-1)/2)
 * Range:     i8 n <= 8, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for up to 128‑bit values using binary search. */
static inline uint64_t isqrt_uint128(__uint128_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n is unsigned; convert to 128‑bit to avoid overflow in 8*n + const. */
    __uint128_t tmpA = (__uint128_t)8 * n + 9;
    __uint128_t tmpB = (__uint128_t)8 * n + 1;

    uint64_t sqrtA = isqrt_uint128(tmpA);
    uint64_t sqrtB = isqrt_uint128(tmpB);

    seq_t A = (seq_t)((sqrtA - 1) / 2);
    seq_t B = (seq_t)((sqrtB - 1) / 2);
    seq_t sn = (seq_t)n;

    /* Term 1: (A^2 + 3*A - 2*n) * (-A^2 + A + 2*(n+4)) */
    seq_t term1_left  = A * A + 3 * A - 2 * sn;
    seq_t term1_right = -(A * A) + A + 2 * (sn + 4);
    seq_t term1 = term1_left * term1_right;

    /* Term 2: (B^2 + 3*B - 2*(n-1)) * (-B^2 + B + 2*(n+3)) */
    seq_t term2_left  = B * B + 3 * B - 2 * (sn - 1);
    seq_t term2_right = -(B * B) + B + 2 * (sn + 3);
    seq_t term2 = term2_left * term2_right;

    return (term1 - term2) / 4;
}
