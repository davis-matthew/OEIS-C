/*
 * A194142  a(n) = Sum_{j=1..n} floor(j*(3-sqrt(3))); n-th partial sum of
 *          Beatty sequence for 3-sqrt(3).
 * https://oeis.org/A194142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*(3-sqrt(3)))
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        uint64_t jj = (uint64_t)j;
        uint64_t sq = jj * jj * 3ULL;
        uint64_t sqrt_floor = isqrt_u64(sq);
        seq_t term = (seq_t)(3ULL * jj) - (seq_t)sqrt_floor - 1;
        sum += term;
    }
    return sum;
}
