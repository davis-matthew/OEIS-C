/*
 * A049416  Largest number whose square has n digits.
 * https://oeis.org/A049416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = ceil(sqrt(10^n)) - 1 = floor(sqrt(10^n - 1))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 26,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* pow10(idx_t) -> seq_t */
#include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t) -> seq_t */
#include <stdint.h>   /* For __uint128_t */

/* 128‑bit power of ten for small seq_t types (≤64 bits) */
static __uint128_t pow10_u128(idx_t e)
{
    __uint128_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* even n: a(n) = 10^(n/2) – 1 */
    if ((n & 1) == 0) {
        idx_t half = n >> 1;
        seq_t res;
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            __uint128_t p = pow10_u128(half);
            res = (seq_t)(p - 1);
        } else {
            res = pow10(half) - 1;
        }
        return res;
    }

    /* odd n: a(n) = floor(sqrt(10^n – 1)) */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* Use 128‑bit arithmetic for n where the result fits in 64 bits */
        __uint128_t target = pow10_u128(n) - 1;
        __uint128_t high   = pow10_u128((n >> 1) + 1);  /* 10^((n+1)/2) */
        __uint128_t low    = 0;

        while (low < high) {
            __uint128_t mid = (low + high) >> 1;
            __uint128_t sq  = mid * mid;
            if (sq <= target)
                low = mid + 1;
            else
                high = mid;
        }
        return (seq_t)(low - 1);
    } else {
        seq_t t = pow10(n) - 1;
        return floor_sqrt(t);
    }
}
