/*
 * A211424  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2+3*x*y=0.
 * https://oeis.org/A211424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with w,x,y∈{−n,…,0,…,n}
 *            and w^2 + 3xy = 0
 * Range:     u8 n <= 26, u16 n <= 3590, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 69 terms through n = 9732 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A211424: the number of ordered triples (w,x,y) in [-n,n]^3 with
 * w^2 + 3xy = 0. w = 0 gives 4n + 1 triples (x = 0 or y = 0); otherwise
 * w = 3k (3 | w^2 forces 3 | w) and xy = -3k^2, so x is +-d for a divisor d of
 * M = 3k^2 with d <= n and M/d <= n, the sign of y following: 2 triples per
 * such d. M's divisors come from factoring k (the original factored M with
 * primes up to 6000 only, too few for M past 3.6e7). */

#include <stdint.h>
#include <stdlib.h>

static uint64_t count_divisors_in_bounds(uint64_t k, uint64_t n)
{
    uint64_t P[16], E[16];
    int np = 0;
    uint64_t t = k;
    P[np] = 3; E[np++] = 1;                   /* the factor 3 of M = 3k^2 */
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p) continue;
        uint64_t e = 0;
        while (t % p == 0) { t /= p; ++e; }
        if (p == 3) E[0] += 2 * e;
        else { P[np] = p; E[np++] = 2 * e; }
    }
    if (t > 1) {
        if (t == 3) E[0] += 2;
        else { P[np] = t; E[np++] = 2; }
    }
    size_t cap = 1;
    for (int i = 0; i < np; ++i) cap *= (size_t)(E[i] + 1);
    uint64_t *div = (uint64_t *)malloc(cap * sizeof *div);
    if (!div) abort();
    size_t nd = 1;
    div[0] = 1;
    for (int i = 0; i < np; ++i) {
        size_t old = nd;
        uint64_t m = 1;
        for (uint64_t e = 1; e <= E[i]; ++e) {
            m *= P[i];
            for (size_t j = 0; j < old; ++j) div[nd++] = div[j] * m;
        }
    }
    const uint64_t M = 3 * k * k;
    uint64_t cnt = 0;
    for (size_t i = 0; i < nd; ++i)
        if (div[i] <= n && M / div[i] <= n) ++cnt;
    free(div);
    return cnt;
}

seq_t a(idx_t n)
{
    const uint64_t N = (uint64_t)n;
    uint64_t total = 4 * N + 1;                /* w = 0 */
    for (uint64_t k = 1; 3 * k <= N; ++k)
        total += 2 * 2 * count_divisors_in_bounds(k, N);   /* w = +-3k */
    return (seq_t)total;
}
