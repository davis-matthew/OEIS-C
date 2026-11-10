/*
 * A097249  a(n) is the number of times we must iterate A097246, starting at
 *          n, before the result is squarefree.
 * https://oeis.org/A097249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is squarefree, otherwise 1 + a(A097246(n));
 *            computed by iterating A097246 until the value is squarefree
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9987 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A097249_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= n; d += 6)
        if (n % d == 0 || n % (d + 2) == 0) return false;
    return true;
}

static uint64_t A097249_nextprime(uint64_t p)
{
    uint64_t c = p + 1;
    while (!A097249_isprime(c)) ++c;
    return c;
}

/* One A097246 step; sets *had to true if m had a square factor. */
static uint64_t A097249_step(uint64_t m, bool *had)
{
    uint64_t res = 1;
    *had = false;
    for (uint64_t d = 2; d * d <= m; d += (d == 2 ? 1 : 2)) {
        if (m % d != 0) continue;
        int e = 0;
        while (m % d == 0) { m /= d; ++e; }
        if (e & 1) res *= d;
        if (e >= 2) {
            *had = true;
            uint64_t np = A097249_nextprime(d);
            for (int i = 0; i < (e >> 1); ++i) res *= np;
        }
    }
    if (m > 1) res *= m;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t cnt = 0;
    for (;;) {
        bool had;
        uint64_t nm = A097249_step(m, &had);
        if (!had) break;
        m = nm;
        ++cnt;
    }
    return (seq_t)cnt;
}
