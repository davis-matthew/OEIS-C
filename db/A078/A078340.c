/*
 * A078340  Least k such that A078142(n) = A078142(n+k).
 * https://oeis.org/A078340
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 such that A078142(n) = A078142(n+k), where
 *            A078142(m) = sum over distinct primes p | m of
 *            ((floor(sqrt(p))+1)^2 - p); computed by direct search over k
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 86 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A078340_floor_sqrt(uint64_t v)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static seq_t A078340_diff(uint64_t p)
{
    uint64_t t = A078340_floor_sqrt(p) + 1;
    uint64_t d = (uint64_t)((unsigned __int128)t * t - p);
    return (seq_t)d;
}

static seq_t A078142_a(uint64_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t temp = n;
    seq_t ans = 0;
    for (uint64_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            ans += A078340_diff(d);
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) ans += A078340_diff(temp);
    return ans;
}

seq_t a(idx_t n)
{
    seq_t target = A078142_a((uint64_t)n);
    uint64_t m = (uint64_t)n + 1;
    for (;; ++m) {
        if (A078142_a(m) == target)
            return (seq_t)(m - (uint64_t)n);
    }
}
