/*
 * A081212  Let r(n,k) = if k=0 then n else r(A081210(n),k-1), then a(n) =
 *          Min{i:r(n,i) = r(n,i+1)}.
 * https://oeis.org/A081212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times A081210 must be applied to n to reach a
 *            fixed point (iterate f = A081210 until f(m) = m, counting steps)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9551 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool A081212_sqfree(uint64_t k)
{
    if (k == 0) return false;
    for (uint64_t d = 2; d * d <= k; ++d) {
        if (k % (d * d) == 0) return false;
    }
    return true;
}

static uint64_t A081212_step(uint64_t n)
{
    if (n <= 1) return n;
    uint64_t m = n;
    uint64_t res = 1;
    for (uint64_t p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t pe = 1;
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                pe *= p;
                ++e;
            }
            if (e == 1) {
                res *= p;
            } else {
                uint64_t k = pe;
                while (!A081212_sqfree(k)) --k;
                res *= k;
            }
        }
    }
    if (m > 1) res *= m;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t cnt = 0;
    for (;;) {
        uint64_t y = A081212_step(x);
        if (y == x) break;
        x = y;
        ++cnt;
    }
    return (seq_t)cnt;
}
