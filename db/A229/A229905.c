/*
 * A229905  Number of binary words of length n with exactly one occurrence of
 *          the subword given by the binary expansion of n.
 * https://oeis.org/A229905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n with exactly one
 *            occurrence of the binary expansion of n (OEIS A229905)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_PATTERN_LENGTH 65  /* n is at most 2^64-1, so bits <= 64 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 0;

    /* Determine pattern length m: bits of n */
    idx_t tmp = n;
    unsigned m = 0;
    while (tmp > 0) {
        m++;
        tmp >>= 1;
    }

    /* For n > 0, m <= n always, but keep the guard for completeness */
    if (m > n)
        return 0;

    /* Extract pattern bits: most significant bit first */
    unsigned char pattern[MAX_PATTERN_LENGTH];
    for (unsigned i = 0; i < m; i++)
        pattern[i] = (n >> (m - 1 - i)) & 1U;

    /* Compute KMP prefix function */
    unsigned pi[MAX_PATTERN_LENGTH];
    pi[0] = 0;
    for (unsigned i = 1; i < m; i++) {
        unsigned k = pi[i - 1];
        while (k > 0 && pattern[k] != pattern[i])
            k = pi[k - 1];
        if (pattern[k] == pattern[i])
            k++;
        pi[i] = k;
    }
    unsigned border = (m > 0) ? pi[m - 1] : 0;  /* longest proper prefix-suffix */

    /* Precompute state transitions and flag whether a full match occurs */
    unsigned next_state[MAX_PATTERN_LENGTH][2];
    bool full_match[MAX_PATTERN_LENGTH][2];
    for (unsigned s = 0; s < m; s++) {
        for (int b = 0; b <= 1; b++) {
            unsigned t = s;
            while (t > 0 && pattern[t] != (unsigned)b)
                t = pi[t - 1];
            if (pattern[t] == (unsigned)b)
                t++;
            if (t == m) {                  /* full pattern matched */
                full_match[s][b] = true;
                next_state[s][b] = border; /* state after consuming the match */
            } else {
                full_match[s][b] = false;
                next_state[s][b] = t;
            }
        }
    }

    /* DP arrays: dp0[state] – strings processed so far with 0 matches
       dp1[state] – strings processed so far with exactly 1 match      */
    seq_t dp0[MAX_PATTERN_LENGTH] = {0};
    seq_t dp1[MAX_PATTERN_LENGTH] = {0};
    seq_t ndp0[MAX_PATTERN_LENGTH];
    seq_t ndp1[MAX_PATTERN_LENGTH];

    dp0[0] = 1;      /* start with empty string, 0 matches, state 0 */

    for (idx_t pos = 0; pos < n; pos++) {
        /* reset ndp arrays */
        for (unsigned i = 0; i < m; i++) {
            ndp0[i] = 0;
            ndp1[i] = 0;
        }

        /* transitions from dp0 */
        for (unsigned s = 0; s < m; s++) {
            if (dp0[s] != 0) {
                for (int b = 0; b <= 1; b++) {
                    if (full_match[s][b]) {
                        ndp1[next_state[s][b]] += dp0[s];
                    } else {
                        ndp0[next_state[s][b]] += dp0[s];
                    }
                }
            }
        }

        /* transitions from dp1 (cannot allow second match) */
        for (unsigned s = 0; s < m; s++) {
            if (dp1[s] != 0) {
                for (int b = 0; b <= 1; b++) {
                    if (full_match[s][b]) {
                        /* would produce second match – skip */
                        continue;
                    } else {
                        ndp1[next_state[s][b]] += dp1[s];
                    }
                }
            }
        }

        /* copy ndp to dp for next position */
        for (unsigned i = 0; i < m; i++) {
            dp0[i] = ndp0[i];
            dp1[i] = ndp1[i];
        }
    }

    /* Sum over all states with exactly one match */
    seq_t result = 0;
    for (unsigned s = 0; s < m; s++)
        result += dp1[s];

    return result;
}
