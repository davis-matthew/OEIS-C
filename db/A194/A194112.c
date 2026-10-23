/*
 * A194112  a(n) = Sum_{j=1..n} floor(j*sqrt(8)); n-th partial sum of Beatty
 *          sequence for sqrt(8).
 * https://oeis.org/A194112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*sqrt(8))
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 32‑bit values using binary search. */
static inline uint32_t isqrt_u32(uint32_t x)
{
    uint32_t low = 0, high = x, ans = 0;
    while (low <= high) {
        uint32_t mid = (low + high) >> 1;
        uint64_t sq = (uint64_t)mid * (uint64_t)mid;
        if (sq == x) {
            return mid;
        } else if (sq < x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        /* j is uint64_t, product fits in 64 bits for n up to about 10000. */
        uint64_t prod = (uint64_t)8 * j * j;
        uint32_t term = isqrt_u32((uint32_t)prod);
        sum += (seq_t)term;
    }
    return sum;
}
