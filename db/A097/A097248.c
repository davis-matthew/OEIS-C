/*
 * A097248  a(n) is the eventual stable point reached when iterating k ->
 *          A097246(k), starting from k = n.
 * https://oeis.org/A097248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fixed point of iterating k -> A097246(k) from n; A097246
 *            replaces each p^2 in n by nextprime(p), so the iteration ends at
 *            a squarefree number
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9577 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool A097248_isp(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= n; d += 6)
        if (n % d == 0 || n % (d + 2) == 0) return false;
    return true;
}

static uint64_t A097248_nextp(uint64_t p)
{
    uint64_t c = p + 1;
    while (!A097248_isp(c)) ++c;
    return c;
}

/* one step of A097246; sets *changed if any square factor was replaced */
static uint64_t A097248_step(uint64_t m, bool *changed)
{
    uint64_t res = 1;
    *changed = false;
    for (uint64_t d = 2; d * d <= m; d += (d == 2 ? 1 : 2)) {
        if (m % d != 0) continue;
        int e = 0;
        while (m % d == 0) { m /= d; ++e; }
        if (e & 1) res *= d;
        if (e >= 2) {
            uint64_t np = A097248_nextp(d);
            for (int i = 0; i < (e >> 1); ++i) res *= np;
            *changed = true;
        }
    }
    if (m > 1) res *= m;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    if (k <= 1) return (seq_t)1;
    for (;;) {
        bool ch;
        uint64_t nk = A097248_step(k, &ch);
        if (!ch) break;
        k = nk;
    }
    return (seq_t)k;
}
