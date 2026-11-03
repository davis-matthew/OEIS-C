/*
 * A047717  Numbers that are the sum of 6 but no fewer nonzero fourth powers.
 * https://oeis.org/A047717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that can be expressed as the
 *            sum of exactly 6 non‑zero fourth powers and cannot be expressed
 *            as the sum of fewer than 6 such powers
 * Range:     u8 n <= 18, u16 n <= 7762, u32 n <= 26675, u64 n <= 26675,
 *            u128 n <= 26675, big any n
 * Validated: standard, pass, 69 terms through n = 9892 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047717: numbers that are the sum of 6 but no fewer nonzero fourth powers.
 * dp[x] = the least number of nonzero fourth powers summing to x (capped at 7),
 * by an unbounded knapsack up to a bound; the terms are the x with dp[x] = 6.
 * The bound doubles until it holds the term asked for (the original kept a
 * fixed 10000 terms). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *terms = NULL;
static size_t term_len = 0;

static void build(uint64_t bound)
{
    unsigned char *dp = (unsigned char *)malloc((size_t)bound + 1);
    if (!dp) abort();
    memset(dp, 7, (size_t)bound + 1);
    dp[0] = 0;
    for (uint64_t k = 1; k * k * k * k <= bound; ++k) {
        uint64_t v = k * k * k * k;
        for (uint64_t x = v; x <= bound; ++x)
            if (dp[x - v] + 1 < dp[x]) dp[x] = (unsigned char)(dp[x - v] + 1);
    }
    size_t cnt = 0;
    for (uint64_t x = 1; x <= bound; ++x) cnt += dp[x] == 6;
    free(terms);
    terms = (uint64_t *)malloc((cnt ? cnt : 1) * sizeof *terms);
    if (!terms) abort();
    term_len = 0;
    for (uint64_t x = 1; x <= bound; ++x)
        if (dp[x] == 6) terms[term_len++] = x;
    free(dp);
}

seq_t a(idx_t n)
{
    static uint64_t bound = 0;
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        bound = bound ? 2 * bound : 1 << 20;
        if (bound > (UINT64_C(1) << 34)) return (seq_t)0;
        build(bound);
    }
    return (seq_t)terms[n - 1];
}
