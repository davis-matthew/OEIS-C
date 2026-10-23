/*
 * A119347  Number of distinct sums of distinct divisors of n. Here 0 (as the
 *          sum of an empty subset) is excluded from the count.
 * https://oeis.org/A119347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑empty distinct sums that can be * formed by
 *            adding distinct divisors of n. Excludes the sum 0
 * Range:     u8 n <= 107, u16 n <= 9582, u32 n <= 9582, u64 n <= 9582,
 *            u128 n <= 9582, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void factor_u64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    if (n == 0) { *cnt = 0; return; }
    while ((n & 1ULL) == 0ULL) {          /* factor 2 */
        int e = 0;
        while ((n & 1ULL) == 0ULL) { n >>= 1; ++e; }
        pr[c] = 2ULL; ex[c] = e; ++c;
    }
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) {
            int e = 0;
            while (n % d == 0ULL) { n /= d; ++e; }
            pr[c] = d; ex[c] = e; ++c;
        }
    }
    if (n > 1ULL) { pr[c] = n; ex[c] = 1; ++c; }
    *cnt = c;
}

static void generate_divisors(const uint64_t *pr, const int *ex,
                              size_t pcnt, uint64_t *out, size_t *outcnt)
{
    *outcnt = 1;
    out[0] = 1ULL;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i];
        int e = ex[i];
        size_t curcnt = *outcnt;
        for (int k = 1; k <= e; ++k) {
            uint64_t pw = 1ULL;
            for (int j = 0; j < k; ++j) pw *= p;
            for (size_t j = 0; j < curcnt; ++j) {
                out[*outcnt] = out[j] * pw;
                ++(*outcnt);
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1ULL) return (seq_t)0;
    uint64_t num = (uint64_t)n;

    uint64_t primes[64];
    int exponents[64];
    size_t pcnt;
    factor_u64(num, primes, exponents, &pcnt);

    uint64_t divisors[256];
    size_t dcnt;
    generate_divisors(primes, exponents, pcnt, divisors, &dcnt);

    /* sum of divisors (sigma) in 64‑bit */
    uint64_t sigma = 0;
    for (size_t i = 0; i < dcnt; ++i) sigma += divisors[i];

    /* DP bitset of reachable sums */
    size_t sz = (size_t)sigma + 1;
    uint8_t *dp = (uint8_t*)malloc(sz);
    if (!dp) return (seq_t)0;
    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t v = divisors[i];
        if (v == 0) continue;
        size_t sv = (size_t)v;
        for (size_t s = sz - 1; s >= sv; --s)
            if (dp[s - sv]) dp[s] = 1;
    }

    uint64_t count = 0;
    for (size_t i = 1; i < sz; ++i)
        if (dp[i]) ++count;

    free(dp);
    return (seq_t)count;
}
