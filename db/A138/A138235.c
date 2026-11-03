/*
 * A138235  a(n) = floor(n*(6 + sqrt(6))/5).
 * https://oeis.org/A138235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(6 + sqrt(6))/5)
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor( sqrt(6 * n * n) ) using integer arithmetic */
static seq_t isqrt6n2(idx_t n) {
    __uint128_t t = (__uint128_t)6 * n * n;          /* 6 * n^2 */
    __uint128_t low = 0;
    __uint128_t high = (__uint128_t)3 * n + 1;       /* sqrt(6)*n < 3*n */
    while (low < high) {
        __uint128_t mid = (low + high) >> 1;
        __uint128_t sq = mid * mid;
        if (sq <= t)
            low = mid + 1;
        else
            high = mid;
    }
    return (seq_t)(low - 1);                        /* floor of sqrt */
}

seq_t a(idx_t n) {
    seq_t s = isqrt6n2(n);                          /* floor(n*sqrt(6)) */
    __uint128_t num = (__uint128_t)s + (__uint128_t)6 * n; /* s + 6*n */
    return (seq_t)(num / 5);                      /* floor((s+6n)/5) */
}
