/*
 * A184625  a(n) = floor((n-h)*s +h), where s=2+sqrt(2) and h=-1/4; complement
 *          of A184624.
 * https://oeis.org/A184625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-h)*s + h), where s = 2 + sqrt(2) and h = -1/4
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t SCALE = ((uint64_t)1 << 48);   /* fixed‑point scaling factor */

static uint64_t sqrt2_scaled  = 0;  /* floor(sqrt(2) * SCALE)          */
static uint64_t s_scaled      = 0;  /* floor((2 + sqrt(2)) * SCALE)   */
static uint64_t c_scaled      = 0;  /* floor(((1 + sqrt(2))/4) * SCALE) */
static bool     initialized   = false;

static uint64_t
isqrt_u128(__uint128_t x)
{
    uint64_t low  = 0;
    uint64_t high = SCALE * 2;    /* sqrt(2) < 2   */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* sqrt(2) * SCALE */
        __uint128_t target = (__uint128_t)2 * SCALE * SCALE;
        sqrt2_scaled = isqrt_u128(target);

        /* (2 + sqrt(2)) * SCALE */
        s_scaled = 2 * SCALE + sqrt2_scaled;

        /* ((1 + sqrt(2))/4) * SCALE  */
        uint64_t A = SCALE + sqrt2_scaled;
        c_scaled = (A >> 2) + ((A & 3) == 3 ? 1 : 0);

        initialized = true;
    }

    if (n == 0) /* below the OEIS offset – undefined, defined as 0 */
        return 0;

    __uint128_t val = (__uint128_t)n * s_scaled + c_scaled;
    uint64_t result = (uint64_t)(val / SCALE);
    return (seq_t)result;
}
