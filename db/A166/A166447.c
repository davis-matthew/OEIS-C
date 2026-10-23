/*
 * A166447  a(n) = n*round(sqrt(n)).
 * https://oeis.org/A166447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*round(sqrt(n))
 * Range:     u8 n <= 42, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = ( __uint128_t)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    uint64_t k = floor_sqrt_u64((uint64_t)n);
    uint64_t threshold = (uint64_t)(((__uint128_t)k * k + k + 1));
    if ((uint64_t)n >= threshold) k += 1;
    __uint128_t prod = ( __uint128_t)(seq_t)n * ( __uint128_t)(seq_t)k;
    return (seq_t)prod;
}
