/*
 * A043591  Numbers whose base-3 representation has exactly 11 runs.
 * https://oeis.org/A043591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   comb_11, dp_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A043591: Numbers whose base‑3 representation has exactly 11 runs. */
/* The function generates the nth such number (offset 1). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/comb_11.h"
#include "../utils/dp_index.h"

seq_t a(idx_t n)
{
    /* Find the length L of the ternary representation that contains the nth
       number.  Consecutive signatures: 2048 * C(L, 11) numbers per length. */
    unsigned __int128 target = (unsigned __int128)n;
    unsigned __int128 cum_prev = 0;
    int L = 11;
    for (;;) {
        unsigned __int128 count_len = comb_11(L) * 2048u;
        if (cum_prev + count_len >= target) break;
        cum_prev += count_len;
        ++L;
    }
    unsigned __int128 offset = target - cum_prev - 1;   /* zero‑based in this block */

    /* DP table: dp[pos][run][prev] */
    size_t dp_size = ((size_t)(L + 1)) * 12 * 4;
    unsigned __int128 *dp = (unsigned __int128 *)malloc(dp_size * sizeof(*dp));
    if (!dp) return 0;  /* Out of memory fallback */

    /* Base case: at the end of the number, we must have exactly 11 runs. */
    for (int run = 0; run <= 11; ++run) {
        for (int prev = 0; prev <= 3; ++prev) {
            dp[dp_index(L, run, prev, L)] = (run == 11) ? 1u : 0u;
        }
    }

    /* Bottom‑up DP: fill from position L-1 down to 0. */
    for (int pos = L - 1; pos >= 0; --pos) {
        for (int run = 0; run <= 11; ++run) {
            for (int prev = 0; prev <= 3; ++prev) {
                unsigned __int128 sum = 0;
                int start_d = (pos == 0) ? 1 : 0;  /* leading digit cannot be 0 */
                for (int d = start_d; d <= 2; ++d) {
                    int new_run;
                    if (prev == 3) {
                        new_run = 1;
                    } else {
                        new_run = (prev != d) ? run + 1 : run;
                    }
                    if (new_run <= 11) {
                        sum += dp[dp_index(pos + 1, new_run, d, L)];
                    }
                }
                dp[dp_index(pos, run, prev, L)] = sum;
            }
        }
    }

    /* Reconstruct the offset‑th number in lexicographic (numeric) order. */
    unsigned char *digits = (unsigned char *)malloc((size_t)L * sizeof(*digits));
    if (!digits) { free(dp); return 0; }

    int run = 0, prev = 3;  /* prev=3 means no previous digit */
    unsigned __int128 idx = offset;
    for (int pos = 0; pos < L; ++pos) {
        int start_d = (pos == 0) ? 1 : 0;
        for (int d = start_d; d <= 2; ++d) {
            int new_run;
            if (prev == 3) {
                new_run = 1;
            } else {
                new_run = (prev != d) ? run + 1 : run;
            }
            if (new_run > 11) continue;
            unsigned __int128 cnt = dp[dp_index(pos + 1, new_run, d, L)];
            if (idx < cnt) {
                digits[pos] = (unsigned char)d;
                run = new_run;
                prev = d;
                break;
            } else {
                idx -= cnt;
            }
        }
    }

    /* Convert base‑3 digits to the sequence value. */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 3 + (seq_t)digits[i];
    }

    free(dp);
    free(digits);
    return value;
}
