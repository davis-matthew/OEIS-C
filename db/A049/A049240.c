/*
 * A049240  Smallest nonnegative value taken on by x^2 - n*y^2 for an infinite
 *          number of integer pairs (x, y).
 * https://oeis.org/A049240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is a perfect square; 1 otherwise
 * Range:     u8 n <= 9423, u16 n <= 9423, u32 n <= 9423, u64 n <= 9423,
 *            u128 n <= 9423, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;           /* sqrt(UINT64_MAX) <= 2^32-1 */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {                     /* avoid division by zero */
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {               /* mid*mid <= n without overflow */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);
    return (seq_t)(r * r == n ? 0 : 1);
}
