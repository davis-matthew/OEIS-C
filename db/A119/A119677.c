/*
 * A119677  a(n) is the number of complete squares that fit inside the circle
 *          with radius n, drawn on squared paper at (0, 0).
 * https://oeis.org/A119677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * sum_{x=1}^{n-1} floor(sqrt(n^2 - x^2))
 * Range:     u8 n <= 9, u16 n <= 145, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where x and n are non‑negative integers and the square root is an integer floor.   */
/* This counts all axis‑aligned unit squares that fit entirely inside a circle of radius n.   */
#include <stdint.h>   /* for uint64_t */
#include <stddef.h>   /* for size_t */

/* integer square root for 64‑bit unsigned values */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0;
    idx_t high = v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t prod = (__uint128_t)mid * (__uint128_t)mid;
        if (prod <= v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* calculate n^2 once to reduce repeated multiplication */
    idx_t n_sq = n * n;

    /* iterate over positive x values (first quadrant) */
    for (idx_t x = 1; x < n; ++x) {
        idx_t x_sq = x * x;
        idx_t diff = n_sq - x_sq;
        idx_t cnt = isqrt_u64(diff);
        sum += cnt;
    }
    /* multiply by 4 to account for the four quadrants */
    return sum << 2;
}
