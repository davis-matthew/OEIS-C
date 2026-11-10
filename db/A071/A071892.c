/*
 * A071892  a(n) = lcm(A071888(n), n).
 * https://oeis.org/A071892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(A071888(n), n), where A071888(1)=2 and A071888(n) =
 *            least squarefree m > n with gcd(m,n) > 1; computed by search
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8525 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A071892_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static bool A071892_sqfree(uint64_t x)
{
    for (uint64_t d = 2; d * d <= x; ++d) {
        if (x % (d * d) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t m;
    if (n64 == 1) {
        m = 2;
    } else {
        m = n64 + 1;
        for (;; ++m) {
            if (A071892_gcd(m, n64) > 1 && A071892_sqfree(m)) break;
        }
    }
    uint64_t g = A071892_gcd(m, n64);
    return (seq_t)(m / g) * (seq_t)n64;
}
