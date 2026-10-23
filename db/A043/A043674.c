/*
 * A043674  Numbers whose base-15 representation has exactly 7 runs.
 * https://oeis.org/A043674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑15 representation has exactly 7 runs
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow14_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow14_7.h"

#define MAX_REM 20      /* maximum digits after the first one */

static unsigned long long cnt[MAX_REM + 1][MAX_REM + 2][15];
static int cnt_initialized = 0;

/* precompute cnt[rem][runs][prev] for rem <= MAX_REM, runs <= rem+1, prev in 0..14 */
static void init_cnt(void)
{
    if (cnt_initialized)
        return;
    cnt_initialized = 1;

    /* base case: no positions left */
    for (int runs = 0; runs <= MAX_REM + 1; ++runs)
        for (int prev = 0; prev < 15; ++prev)
            cnt[0][runs][prev] = (runs == 0) ? 1ULL : 0ULL;

    for (int rem = 1; rem <= MAX_REM; ++rem) {
        for (int runs = 0; runs <= rem + 1; ++runs) {
            for (int prev = 0; prev < 15; ++prev) {
                __uint128_t sum = 0;
                for (int d = 0; d < 15; ++d) {
                    int delta = (d != prev);
                    int rr = runs - delta;
                    if (rr < 0 || rr > rem - 1)
                        continue;
                    sum += cnt[rem - 1][rr][d];
                    if (sum > ( (__uint128_t) ~0ULL ))
                        sum = (__uint128_t) ~0ULL;
                }
                cnt[rem][runs][prev] = (unsigned long long)sum;
            }
        }
    }
}

/* ordinary binomial coefficient nCk, n < 64, k < 8 */
static unsigned long long binom(unsigned long long n, unsigned int k)
{
    if (k > n || k > 6)
        return 0;
    __uint128_t res = 1;
    for (unsigned int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return (unsigned long long)res;
}

/* main sequence generator */
seq_t a(idx_t n)
{
    init_cnt();

    if (n == 0)   /* out of range, but behaviour is undefined */
        return 0;

    /* find length L with cumulative >= n */
    unsigned long long cumulative = 0;
    unsigned long long pow14 = pow14_7();
    unsigned long long L = 7;          /* minimal length possible */
    while (1) {
        /* count of numbers of exact length L */
        unsigned long long comb = binom(L - 1, 6);
        __uint128_t cntL128 = (__uint128_t)pow14 * comb;
        unsigned long long cntL;
        if (cntL128 > (~0ULL))
            cntL = ~0ULL;
        else
            cntL = (unsigned long long)cntL128;

        if ((__uint128_t)cumulative + cntL128 >= n)
            break;
        cumulative += cntL;
        ++L;
    }

    unsigned long long k = n - cumulative;   /* 1‑based index within length L */

    seq_t number = 0;
    int prev_digit = -1;   /* undefined for first position */
    unsigned long long runs_left = 7;
    unsigned long long rem = L;   /* positions left including current */

    for (unsigned long long pos = 0; pos < L; ++pos) {
        int start_digit = (pos == 0) ? 1 : 0;
        unsigned long long rem_positions = L - pos - 1;   /* after picking current */

        for (int d = start_digit; d < 15; ++d) {
            unsigned long long delta = (pos == 0) ? 1 : ((d != prev_digit) ? 1 : 0);
            if (runs_left < delta)
                continue;
            unsigned long long runs_after = runs_left - delta;

            unsigned long long ways = 0;
            if (rem_positions <= MAX_REM && runs_after <= rem_positions + 1)
                ways = cnt[rem_positions][runs_after][d];
            else
                ways = 0ULL;   /* impossible state */

            if (k <= ways) {
                /* pick this digit */
                number = number * (seq_t)15 + (seq_t)d;
                prev_digit = d;
                runs_left = runs_after;
                break;
            } else {
                k -= ways;
            }
        }
    }

    return number;
}
