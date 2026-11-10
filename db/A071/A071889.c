/*
 * A071889  a(n) = gcd(A071888(n), n).
 * https://oeis.org/A071889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A071888(n), n), where A071888(1)=2 and A071888(n) =
 *            least squarefree m > n with gcd(m,n) > 1 for n > 1; computed by
 *            direct search
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9706 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A071071_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static bool A071888_sqfree(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % (p * p) == 0) return false;
    }
    return true;
}

static uint64_t A071888_val(uint64_t n)
{
    if (n == 1) return 2;
    for (uint64_t m = n + 1; ; ++m) {
        if (A071071_gcd(m, n) > 1 && A071888_sqfree(m)) return m;
    }
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    return (seq_t)A071071_gcd(A071888_val(n64), n64);
}
