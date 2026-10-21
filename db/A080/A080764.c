/*
 * A080764  First differences of A049472, floor(n/sqrt(2)).
 * https://oeis.org/A080764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2) / sqrt(2)) - floor((n+1) / sqrt(2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128
   returns floor(sqrt(x)) */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 63), res = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* floor(n / sqrt(2)) computed using integer arithmetic */
static seq_t floor_div_sqrt2(idx_t n)
{
    unsigned __int128 square = (unsigned __int128)n * n;
    square /= 2;                     /* square = floor(n^2 / 2) */
    uint64_t root = isqrt_uint128(square); /* floor(sqrt(floor(n^2/2))) */
    return (seq_t)root;
}

seq_t a(idx_t n)
{
    seq_t v1 = floor_div_sqrt2(n + 2);
    seq_t v2 = floor_div_sqrt2(n + 1);
    return v1 - v2;
}
