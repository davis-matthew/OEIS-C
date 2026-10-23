/*
 * A043662  Numbers whose base-13 representation has exactly 7 runs.
 * https://oeis.org/A043662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th smallest number whose base‑13 representation has
 *            exactly 7 runs
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* 12⁷  (used for counting numbers of a fixed length) */
static const idx_t POW12_7 = 35831808ULL;

/* maximum length needed for the problem limits ( > enough for n up to 2⁶⁴ ).
   For n up to 10⁶⁴ the required length is below 250. */
#define MAX_LEN 250

/* dp[rem][runs_needed][prev] = number of ways to fill `rem` positions
   when the previous (already chosen) digit is `prev` and we still need
   exactly `runs_needed` new runs.  runs_needed is 0…6. */
static idx_t dp[MAX_LEN + 1][7][13];
static int dp_initialized = 0;

/* initialise dp table once ------------------------------------------------- */
static void init_dp(void)
{
    if (dp_initialized) return;
    /* base: no remaining positions */
    for (int prev = 0; prev < 13; ++prev) {
        dp[0][0][prev] = 1;
        for (int r = 1; r < 7; ++r) dp[0][r][prev] = 0;
    }

    /* build for all remaining lengths */
    for (int rem = 1; rem <= MAX_LEN; ++rem) {
        for (int r = 0; r < 7; ++r) {
            for (int prev = 0; prev < 13; ++prev) {
                __int128 sum = 0;
                for (int d = 0; d < 13; ++d) {
                    if (d == prev) {
                        sum += dp[rem - 1][r][d];
                    } else if (r > 0) {
                        sum += dp[rem - 1][r - 1][d];
                    }
                }
                if (sum > (__int128)UINT64_MAX) dp[rem][r][prev] = UINT64_MAX;
                else                           dp[rem][r][prev] = (idx_t)sum;
            }
        }
    }
    dp_initialized = 1;
}

/* binomial C(n,6) for n ≥ 6, using 128‑bit intermediate */
static inline idx_t choose6(idx_t n)
{
    if (n < 6) return 0;
    __int128 r = 1;
    for (idx_t i = 0; i < 6; ++i) r = r * (n - i);
    r /= 720;  /* 6! */
    return (idx_t)(r > (__int128)UINT64_MAX ? UINT64_MAX : r);
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_dp();                      /* make sure dp is ready */

    /* 1. find the length L that contains the n‑th number */
    idx_t remaining = n;            /* rank within the current length (1‑based) */
    idx_t L = 7;                    /* minimal length */
    const idx_t powK = POW12_7;     /* 12⁷ */

    while (1) {
        idx_t comb = choose6(L - 1);               /* C(L-1,6) */
        __int128 tot = (__int128)comb * powK;
        idx_t total = (tot > (__int128)UINT64_MAX) ? UINT64_MAX : (idx_t)tot;
        if (remaining <= total) break;
        remaining -= total;
        ++L;
    }
    /* remaining now holds the rank (1‑based) within strings of length L */

    idx_t rank = remaining;
    int prev_digit = -1;            /* sentinel for the first position */
    seq_t val = 0;
    idx_t runs_used = 0;            /* number of runs used up to current position */

    for (idx_t pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;          /* first digit cannot be 0 */
        for (int d = start; d <= 12; ++d) {
            if (pos > 0 && d == prev_digit) continue;

            /* how many runs are used after choosing this digit? */
            idx_t new_runs_used = runs_used;
            if (pos == 0 || d != prev_digit) new_runs_used = (runs_used == 0) ? 1 : runs_used + 1;
            if (new_runs_used > 7) continue;    /* impossible */

            idx_t runs_needed = 7 - new_runs_used;
            idx_t rem = L - pos - 1;            /* positions still to fill */

            idx_t ways = dp[rem][runs_needed][d];   /* number of completions */

            if (rank > ways) {
                rank -= ways;   /* skip all strings starting with this digit */
                continue;
            }

            /* choose this digit */
            val = val * 13 + (seq_t)d;
            prev_digit = d;
            runs_used = new_runs_used;
            break;
        }
    }
    return val;
}
