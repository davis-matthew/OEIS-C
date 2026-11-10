/*
 * A229302  Numbers n such that A031971(6*n) == n (mod 6*n).
 * https://oeis.org/A229302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A031971(6k) == k (mod 6k), where
 *            A031971(m) = Sum_{j=1..m} j^m; found by direct search, computing
 *            the sum mod 6k with modular powers
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A229302_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    unsigned __int128 r = 1 % m, x = b % m;
    while (e) {
        if (e & 1) r = (r * x) % m;
        x = (x * x) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

static int A229302_test(uint64_t k)
{
    uint64_t m = 6 * k;
    uint64_t s = 0;
    for (uint64_t j = 1; j <= m; ++j) {
        s += A229302_powmod(j, m, m);
        if (s >= m) s -= m;
    }
    return s == k % m;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, k = 0;
    while (cnt < (uint64_t)n) {
        ++k;
        if (A229302_test(k)) ++cnt;
    }
    return (seq_t)k;
}
