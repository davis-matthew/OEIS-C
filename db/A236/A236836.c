/*
 * A236836  The least inverse of A234741: a(n) = the smallest k such that
 *          A234741(k) = n, and 0 if no such k exists.
 * https://oeis.org/A236836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A234741(k) = n (0 if none); search over
 *            GF(2)[x] factorizations of n into factors whose integer
 *            encodings are prime, minimizing the product of those primes.
 *            a(0)=0
 * Range:     u8 n <= 125, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 86 terms through n = 7937 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A236836_bl(uint64_t x)
{
    int b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

static bool A236836_isprime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

/* GF(2) polynomial division: returns true if p divides m, quotient in *q */
static bool A236836_pdiv(uint64_t m, uint64_t p, uint64_t *q)
{
    uint64_t r = m, qq = 0;
    int dp = A236836_bl(p);
    int dr = A236836_bl(r);
    while (dr >= dp) {
        int s = dr - dp;
        qq |= (uint64_t)1 << s;
        r ^= p << s;
        dr = A236836_bl(r);
    }
    if (r != 0) return false;
    *q = qq;
    return true;
}

/* smallest product of nondecreasing primes (>= minp) whose carryless product is m; 0 if none */
static uint64_t A236836_rec(uint64_t m, uint64_t minp)
{
    if (m == 1) return 1;
    uint64_t best = 0;
    for (uint64_t p = minp; p <= m; ++p) {
        uint64_t q;
        if (!A236836_pdiv(m, p, &q)) continue;
        if (!A236836_isprime(p)) continue;
        uint64_t r = A236836_rec(q, p);
        if (r) {
            uint64_t c = p * r;
            if (!best || c < best) best = c;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return (seq_t)A236836_rec((uint64_t)n, 2);
}
