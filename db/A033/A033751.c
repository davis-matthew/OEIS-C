/*
 * A033751  Expansion of Product_{d | 39} theta_3(q^d).
 * https://oeis.org/A033751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integer solutions (x₁,x₂,x₃,x₄) satisfying n = x₁² +
 *            3·x₂² + 13·x₃² + 39·x₄², counting all sign choices for each
 *            variable separately
 * Range:     u8 n <= 363, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: floor of integer square root for 64‑bit values.  */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq > n)  high = mid - 1;
        else         low  = mid + 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t max_w = floor_sqrt_u64(n / 39u);
    for (idx_t w = 0; w <= max_w; ++w) {
        idx_t rem1 = n - 39u * w * w;
        idx_t max_z = floor_sqrt_u64(rem1 / 13u);
        for (idx_t z = 0; z <= max_z; ++z) {
            idx_t rem2 = rem1 - 13u * z * z;
            idx_t max_y = floor_sqrt_u64(rem2 / 3u);
            for (idx_t y = 0; y <= max_y; ++y) {
                idx_t rem3 = rem2 - 3u * y * y;
                idx_t x = floor_sqrt_u64(rem3);
                if (x * x != rem3) continue;          /* not a perfect square */
                seq_t mult = 1;
                if (w) mult <<= 1;   /* two sign choices for x₄ */
                if (z) mult <<= 1;   /* two sign choices for x₃ */
                if (y) mult <<= 1;   /* two sign choices for x₂ */
                if (x) mult <<= 1;   /* two sign choices for x₁ */
                result += mult;
            }
        }
    }
    return result;
}
