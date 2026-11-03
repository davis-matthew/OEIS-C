/*
 * A043650  Numbers whose base-11 representation has exactly 7 runs.
 * https://oeis.org/A043650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers with base-11 representation having exactly 7 runs
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <limits.h>

static unsigned long long combo6(unsigned long long n) {
    /* compute C(n,6) for n >= 6 */
    if (n < 6) return 0;
    unsigned long long r = 1;
    for (unsigned int i = 1; i <= 6; ++i) {
        r = r * (n - (6 - i)) / i;
    }
    return r;
}

static unsigned long long countWays(idx_t pos, unsigned char runs,
                                    unsigned char last_digit_index,
                                    idx_t L,
                                    unsigned long long *dp) {
    /* dp index: (pos * 8 + runs) * 12 + last_digit_index */
    size_t idx = ((size_t)pos * 8 + runs) * 12 + last_digit_index;
    unsigned long long val = dp[idx];
    if (val != ULLONG_MAX) return val;

    if (pos == L) {
        val = (runs == 7) ? 1 : 0;
        dp[idx] = val;
        return val;
    }

    val = 0;
    for (int d = 0; d <= 10; ++d) {
        if (pos == 0 && d == 0) continue;
        unsigned char new_runs;
        if (last_digit_index == 11) {
            new_runs = 1;                 /* first digit starts a run */
        } else {
            new_runs = (d != last_digit_index) ? (runs + 1) : runs;
        }
        if (new_runs > 7) continue;
        val += countWays(pos + 1, new_runs, (unsigned char)d, L, dp);
    }
    dp[idx] = val;
    return val;
}

seq_t a(idx_t n) {
    /* Find the length L of the number whose index is n */
    idx_t remaining = n;
    int L = 7;
    const unsigned long long RUNS_COUNT = 10000000ULL; /* 10^7  = 10*10^6 */
    for (;;) {
        unsigned long long combos = combo6((unsigned long long)L - 1);
        unsigned long long countL = combos * RUNS_COUNT;
        if (remaining <= countL) break;
        remaining -= countL;
        ++L;
    }
    /* remaining is the 1‑based rank within length L numbers */
    idx_t r = remaining;

    /* DP table: dp[pos][runs][last_digit] */
    size_t dp_size = (size_t)(L + 1) * 8 * 12;
    unsigned long long *dp = (unsigned long long *)calloc(dp_size, sizeof(unsigned long long));
    if (!dp) return (seq_t)0;           /* allocation failed */
    for (size_t i = 0; i < dp_size; ++i) dp[i] = ULLONG_MAX;

    seq_t val = 0;
    int pos = 0;
    unsigned char runs = 0;
    unsigned char last_digit_index = 11; /* none */

    while (pos < L) {
        for (int d = 0; d <= 10; ++d) {
            if (pos == 0 && d == 0) continue;
            unsigned char new_runs;
            if (last_digit_index == 11) {
                new_runs = 1;
            } else {
                new_runs = (d != last_digit_index) ? (runs + 1) : runs;
            }
            if (new_runs > 7) continue;
            unsigned long long cnt = countWays(pos + 1, new_runs, (unsigned char)d,
                                               L, dp);
            if (r <= cnt) {
                val = val * 11 + d;
                runs = new_runs;
                last_digit_index = (unsigned char)d;
                break;
            } else {
                r -= cnt;
            }
        }
        ++pos;
    }

    free(dp);
    return val;
}
