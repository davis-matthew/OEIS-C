/*
 * A039427  Numbers whose base-10 representation has the same nonzero number
 *          of 4's and 9's.
 * https://oeis.org/A039427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: Enumerates numbers whose decimal digits contain an equal,
   non‑zero number of 4's and 9's, sorted ascending. */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Helper: binomial coefficient with an upper bound (cap). */
static uint64_t binom(uint64_t n, uint64_t k, uint64_t limit)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return 1;

    __int128 res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res *= (__int128)(n - k + i);
        res /= i;
        if (res > limit) return limit + 1;
    }
    return (uint64_t)res;
}

/* Helper: 8^exp with an upper bound (cap). */
static uint64_t pow8(uint64_t exp, uint64_t limit)
{
    if (exp == 0) return 1;
    __int128 res = 1;
    for (uint64_t i = 0; i < exp; ++i) {
        res *= 8;
        if (res > limit) return limit + 1;
    }
    return (uint64_t)res;
}

/* Count how many completions exist for a given prefix. */
static uint64_t count_remaining(uint64_t rem,
                                uint64_t c4,
                                uint64_t c9,
                                uint64_t limit)
{
    if (rem == 0)
        return (c4 == c9 && c4 > 0) ? 1 : 0;

    uint64_t total = 0;
    uint64_t t_min = (c4 > c9) ? c4 : c9;
    if (t_min == 0) t_min = 1;
    uint64_t t_max = (rem + c4 + c9) / 2;
    if (t_min > t_max) return 0;

    for (uint64_t t = t_min; t <= t_max; ++t) {
        uint64_t delta4 = t - c4;
        uint64_t delta9 = t - c9;
        uint64_t sumdelta = delta4 + delta9;
        if (sumdelta > rem) continue;

        uint64_t ways4 = binom(rem, delta4, limit);
        if (ways4 == 0) continue;
        uint64_t ways9 = binom(rem - delta4, delta9, limit);
        if (ways9 == 0) continue;
        uint64_t ways8 = pow8(rem - sumdelta, limit);
        if (ways8 == 0) continue;

        __int128 prod = (__int128)ways4 * ways9;
        if (prod > limit) prod = limit + 1;
        prod *= ways8;
        if (prod > limit) prod = limit + 1;

        uint64_t prod_u = (uint64_t)prod;
        total += prod_u;
        if (total > limit) { total = limit + 1; break; }
    }
    return total;
}

/* Count how many valid numbers exist with exactly L digits. */
static uint64_t count_by_length(uint64_t L, uint64_t limit)
{
    if (L == 0) return 0;
    uint64_t total = 0;
    for (uint64_t d = 1; d <= 9; ++d) {
        uint64_t c4 = (d == 4) ? 1 : 0;
        uint64_t c9 = (d == 9) ? 1 : 0;
        uint64_t rem = L - 1;
        uint64_t ways = count_remaining(rem, c4, c9, limit);
        total += ways;
        if (total > limit) { total = limit + 1; break; }
    }
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for offset <1 */

    /* Find the minimal length L such that the cumulative count reaches n. */
    uint64_t cum = 0;
    uint64_t L = 1;
    while (true) {
        uint64_t cnt = count_by_length(L, (uint64_t)n);
        if (cum + cnt >= n) break;
        cum += cnt;
        ++L;
    }

    uint64_t k = n - cum;                 /* rank within length L */
    seq_t result = 0;
    uint64_t c4 = 0, c9 = 0;

    for (uint64_t pos = 0; pos < L; ++pos) {
        int start_digit = (pos == 0) ? 1 : 0;
        for (int d = start_digit; d <= 9; ++d) {
            uint64_t nc4 = c4 + (d == 4);
            uint64_t nc9 = c9 + (d == 9);
            uint64_t rem = L - pos - 1;
            uint64_t ways = count_remaining(rem, nc4, nc9, k);
            if (ways >= k) {
                result = result * 10 + (seq_t)d;
                c4 = nc4;
                c9 = nc9;
                break;
            } else {
                k -= ways;
            }
        }
    }
    return result;
}
