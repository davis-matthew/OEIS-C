/*
 * A043626  Numbers whose base-8 representation has exactly 4 runs.
 * https://oeis.org/A043626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The n-th integer whose base‑8 representation contains exactly four runs. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Find the length L of the base‑8 representation. */
    idx_t L = 4;
    unsigned long long cumulative = 0;
    unsigned long long countL;
    for (;; ++L) {
        unsigned long long comb = (unsigned long long)(L - 1) *
                                (L - 2) * (L - 3) / 6;
        countL = 2401ULL * comb;            /* 7^4 * C(L-1,3) */
        if (cumulative + countL >= n) break;
        cumulative += countL;
    }

    /* 0‑based index within the block of length L. */
    unsigned long long offset = n - cumulative - 1;

    /* Pre‑compute DP[rem][runs] for 0 ≤ rem ≤ L−1, 0 ≤ runs ≤ 4. */
    enum { MAXLEN = 200 };
    static unsigned long long dp[MAXLEN + 1][5];
    /* Base case: no positions left → must have used all 4 runs. */
    for (int r = 0; r <= 4; ++r) dp[0][r] = (r == 4) ? 1ULL : 0ULL;

    for (int rem = 1; rem <= (int)L - 1; ++rem) {
        dp[rem][4] = dp[rem - 1][4];            /* cannot start a new run */
        for (int r = 3; r >= 1; --r)
            dp[rem][r] = dp[rem - 1][r] +
                         7ULL * dp[rem - 1][r + 1]; /* new run */
        dp[rem][0] = dp[rem - 1][0] + 7ULL * dp[rem - 1][1];
    }

    /* Choose the first digit (1..7). */
    unsigned long long per_first = dp[L - 1][1];  /* completions for a given first digit */
    unsigned long long first_idx = offset / per_first;
    offset %= per_first;
    uint8_t digits[200];
    digits[0] = (uint8_t)(1 + (unsigned)first_idx);

    int runs_used = 1;                 /* the first run */
    uint8_t prev_digit = digits[0];

    /* Generate remaining digits. */
    for (int pos = 1; pos < (int)L; ++pos) {
        int rem_after = (int)(L - pos - 1);
        for (uint8_t d = 0; d <= 7; ++d) {
            unsigned long long cnt;
            if (d == prev_digit) {
                cnt = dp[rem_after][runs_used];
            } else {
                if (runs_used == 4) continue;           /* cannot start a fifth run */
                cnt = dp[rem_after][runs_used + 1];
            }
            if (offset < cnt) {
                digits[pos] = d;
                if (d != prev_digit) ++runs_used;
                prev_digit = d;
                break;
            } else {
                offset -= cnt;
            }
        }
    }

    /* Convert the base‑8 digits to the required return type. */
    seq_t value = 0;
    for (int i = 0; i < (int)L; ++i) {
        value = value * (seq_t)8 + (seq_t)digits[i];
    }

    return value;
}
