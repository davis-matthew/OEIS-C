/*
 * A068549  Largest prime < sqrt(2n-4) - 1.
 * https://oeis.org/A068549
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   largest prime < sqrt(2n-4) - 1
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer floor square root for uint64_t */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = (x < 0xFFFFFFFFULL) ? x : 0xFFFFFFFFULL;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Trial division test for primality */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* compute 2*n - 4 and its integer floor square root */
    uint64_t val = 2 * n - 4;          /* safe for n ≲ 2^63-1 */
    uint64_t sq  = isqrt_u64(val);
    uint64_t lim = sq - 1;             /* largest integer < sqrt(2n-4) */

    /* find the largest prime strictly less than lim */
    if (lim <= 3) return (seq_t)2;      /* covers n = 10 */
    uint64_t p = lim - 1;
    while (p >= 2) {
        if (is_prime_uint64(p)) return (seq_t)p;
        if (p == 2) break;
        --p;
    }
    return (seq_t)2;                    /* fallback, should not occur for n ≥ 10 */
}
