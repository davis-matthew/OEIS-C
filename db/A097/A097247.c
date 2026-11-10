/*
 * A097247  A097246(A097246(n)).
 * https://oeis.org/A097247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A097246(A097246(n)), where A097246(n) = Product_{p^e ||
 *            n} nextprime(p)^floor(e/2) * p^(e mod 2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool A097247_isp(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= n; d += 6)
        if (n % d == 0 || n % (d + 2) == 0) return false;
    return true;
}

static uint64_t A097247_nextp(uint64_t p)
{
    uint64_t c = p + 1;
    while (!A097247_isp(c)) ++c;
    return c;
}

/* A097246 on native integers; the result never exceeds the argument */
static uint64_t A097247_f(uint64_t m)
{
    uint64_t res = 1;
    if (m <= 1) return 1;
    for (uint64_t d = 2; d * d <= m; d += (d == 2 ? 1 : 2)) {
        if (m % d != 0) continue;
        int e = 0;
        while (m % d == 0) { ++e; m /= d; }
        if (e & 1) res *= d;
        if (e >= 2) {
            uint64_t np = A097247_nextp(d);
            for (int i = 0; i < (e >> 1); ++i) res *= np;
        }
    }
    if (m > 1) res *= m;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t v = A097247_f(A097247_f((uint64_t)n));
    return (seq_t)v;
}
