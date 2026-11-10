/*
 * A078328  Least m such that A078142(m) = A006530(m) = n-th prime.
 * https://oeis.org/A078328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*P where p = prime(n), and P is the least product of
 *            distinct primes q < p with sum of ((floor(sqrt q)+1)^2 - q) over
 *            those q equal to p - ((floor(sqrt p)+1)^2 - p). Computed by a
 *            0/1 knapsack DP over sums that minimizes the product
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 u128_t;

static uint64_t A078328_isqrt(uint64_t v)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= v) r++;
    return r;
}

static uint64_t A078328_cost(uint64_t q)
{
    uint64_t t = A078328_isqrt(q) + 1;
    return t * t - q;
}

static int A078328_isprime(uint64_t x)
{
    if (x < 2) return 0;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, p = 1;
    while (cnt < (uint64_t)n) {
        p++;
        if (A078328_isprime(p)) cnt++;
    }
    uint64_t R = p - A078328_cost(p);
    const u128_t INF = ~(u128_t)0;
    u128_t *dp = (u128_t *)malloc((R + 1) * sizeof(u128_t));
    for (uint64_t i = 0; i <= R; i++) dp[i] = INF;
    dp[0] = 1;
    for (uint64_t q = 2; q < p; q++) {
        if (!A078328_isprime(q)) continue;
        uint64_t c = A078328_cost(q);
        if (c > R) continue;
        for (uint64_t s = R; s >= c; s--) {
            u128_t prev = dp[s - c];
            if (prev == INF) { if (s == c) break; continue; }
            u128_t v;
            if (prev > (INF - 1) / q) v = INF - 1;
            else v = prev * q;
            if (v < dp[s]) dp[s] = v;
            if (s == c) break;
        }
    }
    u128_t best = dp[R];
    free(dp);
    uint64_t hi = (uint64_t)(best >> 64), lo = (uint64_t)best;
    seq_t prod = (seq_t)hi;
    prod = prod << 32;
    prod = prod << 32;
    prod = prod | (seq_t)lo;
    return prod * (seq_t)p;
}
