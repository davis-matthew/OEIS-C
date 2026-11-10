/*
 * A054029  First differences of A054028.
 * https://oeis.org/A054029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = A054028(2) = 2, a(n) = A054028(n+1) - A054028(n) for n >=
 *            2, where A054028(n) = least k with 2^k >= k^n, found by binary
 *            search
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static bool A054029_ok(uint64_t k, uint64_t n)
{
    if (k < 2) return false;
    if ((k & (k - 1)) == 0) {
        uint64_t m = 0;
        while (((uint64_t)1 << m) < k) m++;
        return k >= n * m;
    }
    long double rhs = (long double)n * log2l((long double)k);
    return (long double)k >= rhs;
}

static uint64_t A054029_f(uint64_t n)
{
    if (n <= 2) return 2;
    uint64_t lo = 3, hi = 4;
    while (!A054029_ok(hi, n)) hi <<= 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A054029_ok(mid, n)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)2;
    return (seq_t)(A054029_f((uint64_t)n + 1) - A054029_f((uint64_t)n));
}
