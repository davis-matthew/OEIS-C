/*
 * A228295  The 'Honeycomb' or 'Beehive' sequence: a(n) = ceiling(12^(1/4)*n).
 * https://oeis.org/A228295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(12^(1/4) * n) (i.e. the smallest m with m^4 ≥
 *            12·n^4)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow4_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/pow4_128.h"

seq_t a(idx_t n)
{
    uint64_t u = (uint64_t)n;
    if (u == 0) return (seq_t)0;

    /* target = 12 * u^4  */
    __uint128_t target = 12 * pow4_128(u);

    /* Find an upper bound high such that high^4 >= target. */
    uint64_t low = 0, high = 1;
    while (pow4_128(high) < target) {
        low = high;
        high <<= 1;                     /* double the upper bound */
        if (high == 0) {                /* overflow: saturate at UINT64_MAX */
            high = UINT64_MAX;
            break;
        }
    }

    /* Binary search for the smallest m with m^4 >= target. */
    while (low + 1 < high) {
        uint64_t mid = (low + high) >> 1;
        if (pow4_128(mid) >= target)
            high = mid;
        else
            low = mid;
    }

    return (seq_t)high;
}
