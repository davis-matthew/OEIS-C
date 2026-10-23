/*
 * A164506  Number of binary strings of length n with no substrings equal to
 *          0011 0110 or 1001.
 * https://oeis.org/A164506
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n with no * substrings
 *            equal to 0011, 0110 or 1001. * Computed via a DFA DP with states
 *            corresponding to the prefixes of the * banned patterns
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 171,
 *            big any n
 * Helpers:   is_banned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_banned.h"

#define STATE_COUNT 11
static const char *const state_str[STATE_COUNT] = {
    "", "0", "1", "00", "01", "11", "10", "001", "011", "110", "100"
};
static const int state_len[STATE_COUNT] = {0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};

/* Build the transition table for the DFA. */
static void build_transitions(int trans[STATE_COUNT][2])
{
    char t[5];  /* up to 4 characters + NUL, but NUL not needed */
    for (int s = 0; s < STATE_COUNT; ++s) {
        for (int b = 0; b < 2; ++b) {
            int len_t = state_len[s] + 1;
            /* copy existing state string */
            for (int k = 0; k < state_len[s]; ++k) t[k] = state_str[s][k];
            t[len_t - 1] = '0' + b;
            /* Check banned substrings */
            if (is_banned(t, len_t)) {
                trans[s][b] = -1;
                continue;
            }
            /* Find longest suffix that is a prefix of a banned pattern */
            int best = -1;
            int max_l = len_t < 3 ? len_t : 3;
            for (int l = max_l; l >= 0; --l) {
                for (int j = 0; j < STATE_COUNT; ++j) {
                    if (state_len[j] != l) continue;
                    bool match = true;
                    for (int k = 0; k < l; ++k) {
                        if (t[len_t - l + k] != state_str[j][k]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        best = j;
                        break;
                    }
                }
                if (best != -1) break;
            }
            trans[s][b] = best;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n < 4 the sequence is undefined; return 0. */
    if (n < 4) return 0;

    /* Transition table for all states. */
    static int trans[STATE_COUNT][2];
    static bool built = false;
    if (!built) {
        build_transitions(trans);
        built = true;
    }

    /* DP arrays for current and next length */
    seq_t dp[STATE_COUNT] = {0};
    seq_t next[STATE_COUNT];
    dp[0] = 1; /* start from empty state */

    for (idx_t len = 0; len < n; ++len) {
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp[s] == 0) continue;
            int ns = trans[s][0];
            if (ns != -1) next[ns] += dp[s];
            ns = trans[s][1];
            if (ns != -1) next[ns] += dp[s];
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = next[i];
    }

    /* Sum all states to get total number of valid strings of length n. */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += dp[i];
    return total;
}
