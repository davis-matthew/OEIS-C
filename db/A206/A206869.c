/*
 * A206869  Number of nX6 0..1 arrays avoiding 0 0 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   build_row_patterns, build_transitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n×6 0..1 arrays avoiding horizontal “000” and vertical “101”.
   Length 6 rows; constraints depend only on the last two rows, so a transfer‐matrix
   style DP with 44 allowed row patterns is used.  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_row_patterns.h"
#include "../utils/build_transitions.h"

seq_t a(idx_t n) {
    /* Build the row pattern list once */
    static bool init = false;
    static int pat[44], pat_cnt;
    static int state_idx[44][44];
    static int state_cnt;
    static int next_count[1936];
    static int next_state[1936][44];
    static seq_t dp_curr[1936], dp_next[1936];

    if (!init) {
        build_row_patterns(pat, &pat_cnt);
        build_transitions(pat, pat_cnt, state_idx,
                         &state_cnt, next_count, next_state);
        init = true;
    }

    /* Answers for n = 1 and n = 2 can be returned directly. */
    if (n == 0) return 0;                /* undefined, but safe */
    if (n == 1) {
        seq_t r = (seq_t)pat_cnt;
        return r;
    }
    if (n == 2) {
        seq_t r = (seq_t)pat_cnt * pat_cnt;
        return r;
    }

    /* Initialize dp for length 2: every pair of allowed rows is one way. */
    for (int i = 0; i < state_cnt; ++i) dp_curr[i] = (seq_t)0;
    for (int i = 0; i < pat_cnt; ++i)
        for (int j = 0; j < pat_cnt; ++j)
            dp_curr[state_idx[i][j]] = (seq_t)1;

    /* Iterate from length 3 up to n. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < state_cnt; ++i) dp_next[i] = (seq_t)0;
        for (int old = 0; old < state_cnt; ++old) {
            seq_t val = dp_curr[old];
            if (val == (seq_t)0) continue;
            int kcnt = next_count[old];
            for (int k = 0; k < kcnt; ++k) {
                int newst = next_state[old][k];
                dp_next[newst] += val;
            }
        }
        /* swap current and next */
        for (int i = 0; i < state_cnt; ++i) dp_curr[i] = dp_next[i];
    }

    /* Sum over all states to get total number of arrays of length n. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < state_cnt; ++i) total += dp_curr[i];
    return total;
}
