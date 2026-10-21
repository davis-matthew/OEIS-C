/*
 * A056737  Minimum nonnegative integer m such that n = k*(k+m) for some
 *          positive integer k.
 * https://oeis.org/A056737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Minimum nonnegative integer m such that n = k*(k+m) for
 *            some positive integer k. This is the difference between the
 *            largest divisor d ≤ √n and the smallest divisor t = n/d ≥ √n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t integer_sqrt(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
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
    /* n ≥ 1 (offset 1).  The function returns 0 for perfect squares. */
    idx_t s = integer_sqrt(n);
    idx_t d = s;
    while (d) {
        if (n % d == 0) {
            idx_t t = n / d;
            return (seq_t)(t - d);   /* minimal difference */
        }
        d--;
    }
    return 0;   /* will never be reached */
}
