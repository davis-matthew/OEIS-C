/*
 * A236833  a(n) = number of times n occurs in A234741.
 * https://oeis.org/A236833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multisets of primes p (integers) whose GF(2)[x]
 *            carryless product equals n; a(0)=1 by convention. Computed by
 *            DFS over primes whose polynomial divides the remainder
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 104 terms through n = 7991 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A236833_isprime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

/* returns true if g divides f over GF(2)[x]; quotient in *q */
static bool A236833_pdiv(uint64_t f, uint64_t g, uint64_t *q)
{
    int dg = 63 - __builtin_clzll(g);
    uint64_t r = f, quo = 0;
    while (r != 0) {
        int dr = 63 - __builtin_clzll(r);
        if (dr < dg) break;
        r ^= g << (dr - dg);
        quo |= 1ULL << (dr - dg);
    }
    if (r != 0) return false;
    *q = quo;
    return true;
}

static uint64_t A236833_dfs(uint64_t rem, uint64_t minp)
{
    if (rem == 1) return 1;
    uint64_t total = 0;
    int dr = 63 - __builtin_clzll(rem);
    for (uint64_t p = minp; p <= rem; ++p) {
        int dp = 63 - __builtin_clzll(p);
        if (dp > dr) break;
        uint64_t q;
        if (!A236833_pdiv(rem, p, &q)) continue;
        if (!A236833_isprime(p)) continue;
        total += A236833_dfs(q, p);
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    return (seq_t)A236833_dfs((uint64_t)n, 2);
}
