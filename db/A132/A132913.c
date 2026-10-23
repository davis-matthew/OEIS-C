/*
 * A132913  a(n) = ceiling(sqrt(n) + n^(1/3)).
 * https://oeis.org/A132913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(n) + n^(1/3))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_cuberoot128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_cuberoot128.h"

#define SCALE 20
#define SCALE_PLUS ((uint64_t)1 << SCALE)

/* Integer square root of a 128‑bit number: floor(sqrt(v)) */
static uint64_t floor_sqrt128(__uint128_t v) {
    uint64_t low = 0, high = ((uint64_t)1 << 53); /* sqrt(2^104) < 2^53  */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* scale n to obtain fixed‑point square and cube roots */
    __uint128_t nsq = (__uint128_t)n << (2 * SCALE);
    __uint128_t ncb = (__uint128_t)n << (3 * SCALE);

    /* floor(sqrt(n) * 2^SCALE) and floor(cuberoot(n) * 2^SCALE) */
    uint64_t sqrt_scaled  = floor_sqrt128(nsq);
    uint64_t cbrt_scaled  = floor_cuberoot128(ncb);

    /* sum the scaled values and round up */
    uint64_t sum_scaled = sqrt_scaled + cbrt_scaled;
    uint64_t ceil_scaled = (sum_scaled + (SCALE_PLUS - 1)) >> SCALE;

    return (seq_t)ceil_scaled;
}
