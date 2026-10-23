/*
 * A128132  A natural number transform, companion to A127701.
 * https://oeis.org/A128132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = k if n = k*(k+1)/2 (k≥1), a(n) = -1 if n = k*(k+1)/2 – 1
 *            (k≥2), a(n) = 0 otherwise
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned long long
isqrt_uint128(__uint128_t x) {
    unsigned long long lo = 0, hi = (unsigned long long) -1;
    unsigned long long ans = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* offset is 1, but defensive */
    __uint128_t discr = (__uint128_t)8 * n + 1;
    unsigned long long sqrtv = isqrt_uint128(discr);
    unsigned long long k = (sqrtv - 1) / 2;   /* floor((√(1+8n)-1)/2) */
    __uint128_t t = (__uint128_t)k * (k + 1) / 2;
    if (t < (__uint128_t)n) {          /* adjust to ceil */
        ++k;
        t = (__uint128_t)k * (k + 1) / 2;
    }
    if ((__uint128_t)n == t)            return (seq_t)k;
    if ((__uint128_t)n == t - 1)        return (seq_t)-1;
    return 0;
}
