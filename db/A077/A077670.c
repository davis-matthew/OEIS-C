/*
 * A077670  Last term of row n of A077669.
 * https://oeis.org/A077670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n-th number m > 1 with gcd(m,k) > 1, m not dividing k, k
 *            not dividing m, where k = n-th composite number (search)
 * Range:     u8 n <= 32, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A077670_gcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static bool A077670_composite(uint64_t x)
{
    if (x < 4) return false;
    if ((x & 1) == 0) return true;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t k = 3;
    uint64_t cnt = 0;
    while (cnt < (uint64_t)n) {
        ++k;
        if (A077670_composite(k)) ++cnt;
    }
    uint64_t found = 0;
    for (uint64_t m = 2; ; ++m) {
        if (k % m == 0) continue;
        if (m % k == 0) continue;
        if (A077670_gcd(k, m) > 1) {
            ++found;
            if (found == (uint64_t)n)
                return (seq_t)m;
        }
    }
}
