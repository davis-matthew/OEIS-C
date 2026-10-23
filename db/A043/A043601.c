/*
 * A043601  Numbers whose base-4 representation has exactly 10 runs.
 * https://oeis.org/A043601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th integer whose base‑4 representation has exactly
   10 runs (consecutive equal digit blocks).  The algorithm
   enumerates the allowed base‑4 digit sequences in lexicographic
   (hence numeric) order, using dynamic programming to count
   continuations.  It works for arbitrary unsigned integer types
   seq_t, including arbitrary‑precision integers. */

#include <stdint.h>

#define K 10              /* number of runs */
#define MAX_L 64          /* maximum length of the base–4 representation
                            needed for the required range */

/* multiplicative binomial: C(n,r), for n,r <= 64   */
static seq_t binom_small(idx_t n, idx_t r)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= r; ++i) {
        res = res * (seq_t)(n - r + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Step 1: find the length L of the base‑4 representation
       of the n‑th number with exactly K runs.  We accumulate
       counts of all lengths until the cumulative count reaches
       n.  The count for each length can be computed combinatorially:
            count(L) = 3 * C(L-1, K-1) * 3^(K-1)
       (first digit has 3 choices, each transition between runs
        has 3 choices). */
    seq_t cumulative = 0;
    seq_t pow3_kminus1 = 1;
    for (idx_t i = 0; i < (K - 1); ++i) {
        pow3_kminus1 *= 3;
    }

    idx_t L = K;  /* minimal possible length */
    for (;;) {
        seq_t count =
            (seq_t)3 *
            binom_small(L - 1, K - 1) *
            pow3_kminus1;
        if (cumulative + count >= (seq_t)n) {
            break;
        }
        cumulative += count;
        ++L;
        if (L > MAX_L) {  /* safety guard – should not happen for n <= 10000 */
            return 0;
        }
    }

    /* remaining is the 1‑based index within the block of length L */
    seq_t remaining = (seq_t)n - cumulative;

    /* Step 2: dynamic programming table dp[pos][runs][prev] */
    static seq_t dp[MAX_L + 1][K + 1][4];
    /* base case: last digit already chosen */
    for (idx_t runs = 0; runs <= K; ++runs) {
        for (idx_t prev = 0; prev < 4; ++prev) {
            dp[L][runs][prev] = (runs == K) ? (seq_t)1 : (seq_t)0;
        }
    }
    /* fill table backwards */
    for (idx_t pos = L - 1; pos > 0; --pos) {
        for (idx_t runs = 0; runs <= K; ++runs) {
            for (idx_t prev = 0; prev < 4; ++prev) {
                seq_t sum = 0;
                for (idx_t d = 0; d < 4; ++d) {
                    idx_t new_runs = runs + ((d != prev) ? 1 : 0);
                    if (new_runs > K) continue;
                    sum += dp[pos + 1][new_runs][d];
                }
                dp[pos][runs][prev] = sum;
            }
        }
    }

    /* Step 3: construct the digit sequence */
    uint8_t digits[MAX_L] = {0};
    idx_t cur_pos = 0;       /* position in digits array (0‑based)   */
    idx_t runs_used = 0;
    idx_t prev_digit = 0;

    /* choose first digit (must be 1..3) */
    for (idx_t c = 1; c <= 3; ++c) {
        seq_t cnt = dp[1][1][c];
        if (remaining > cnt) {
            remaining -= cnt;
        } else {
            digits[0] = (uint8_t)c;
            cur_pos = 1;
            runs_used = 1;
            prev_digit = c;
            break;
        }
    }

    /* choose remaining digits */
    while (cur_pos < L) {
        for (idx_t d = 0; d < 4; ++d) {
            idx_t new_runs = runs_used + ((d != prev_digit) ? 1 : 0);
            if (new_runs > K) continue;
            seq_t cnt = dp[cur_pos + 1][new_runs][d];
            if (remaining > cnt) {
                remaining -= cnt;
            } else {
                digits[cur_pos] = (uint8_t)d;
                runs_used = new_runs;
                prev_digit = d;
                ++cur_pos;
                break;
            }
        }
    }

    /* Step 4: convert base‑4 digits to decimal value */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        value = value * (seq_t)4 + (seq_t)digits[i];
    }

    return value;
}
