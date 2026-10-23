/*
 * A195129  a(n) = 2*n - floor(n*sqrt(1/3)).
 * https://oeis.org/A195129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - floor(n*sqrt(1/3))
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for an unsigned __int128 value
   returns floor(sqrt(x)).  The result fits in seq_t because
   sqrt(x) < 2^64, which is always representable by seq_t. */
static seq_t sqrt_uint128(__uint128_t x)
{
    /* the maximum possible result is less than 2^64,
       so we can safely limit the search space to that. */
    __uint128_t high = x;
    const __uint128_t limit = ((__uint128_t)1 << 64) - 1;
    if (high > limit) high = limit;

    __uint128_t low = 0;
    while (low <= high) {
        __uint128_t mid = (low + high) >> 1;
        __uint128_t sq  = mid * mid;
        if (sq == x) return (seq_t)mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)(low - 1);
}

seq_t a(idx_t n)
{
    /* compute floor(n^2 / 3) using 128‑bit arithmetic to avoid overflow */
    __uint128_t sq   = (__uint128_t)n * n;   /* n*n  (fits in 128 bits) */
    __uint128_t val  = sq / 3;               /* floor(n^2 / 3)   */
    seq_t      floor_part = sqrt_uint128(val);   /* floor(sqrt(val)) */
    seq_t      two_n      = (seq_t)2 * (seq_t)n;
    return two_n - floor_part;
}
