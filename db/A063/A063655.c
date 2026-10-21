/*
 * A063655  Smallest semiperimeter of integral rectangle with area n.
 * https://oeis.org/A063655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min_{d|n} (n/d + d) (for n ≥ 1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer square root (floor) using binary search with 128‑bit multiplication */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* a(n) – smallest semiperimeter of an integral rectangle with area n.
   It is the sum of the largest divisor ≤ sqrt(n) and its complementary
   divisor. */
seq_t a(idx_t n)
{
    /* prevent misuse: a(1) is defined; for n == 0 return a(1). */
    if (n == 0)
        return (seq_t)2;

    uint64_t sqrt_n = floor_sqrt_u64(n);

    /* scan downwards from sqrt_n to find the largest divisor ≤ sqrt(n) */
    for (uint64_t d = sqrt_n; d >= 1; --d) {
        if (n % d == 0) {
            uint64_t e = n / d;
            return (seq_t)(d + e);
        }
    }
    /* fallback – should never reach here because d = 1 always divides n */
    return (seq_t)(1 + n);
}
