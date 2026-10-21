/*
 * A208074  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 0 vertically.
 * https://oeis.org/A208074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   ok_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208074: DP over rows; a(1)=10, a(2)=100. */
#include <stdbool.h>
#include "../utils/ok_triple.h"

/* List of allowed 4-bit row patterns that avoid horizontal 000 and 001. */
static const unsigned int rows[10] = {
    4, 5, 6, 7, 10, 11, 12, 13, 14, 15
};

/* DP transition tables: trans_to[state][k] = next state index,
   state = prev_row_index * 10 + cur_row_index. */
static int trans_to[100][10];
static int trans_cnt[100];
static int initialized = 0;

/* Returns the value a(n) for the sequence A208074. */
seq_t a(idx_t n)
{
    /* Small special cases. */
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)100;

    /* Initialise transition tables once. */
    if (!initialized) {
        for (int pi = 0; pi < 10; ++pi) {
            for (int ci = 0; ci < 10; ++ci) {
                int state = pi * 10 + ci;
                int cnt = 0;
                for (int ni = 0; ni < 10; ++ni) {
                    if (ok_triple(rows[pi], rows[ci], rows[ni])) {
                        trans_to[state][cnt++] = ci * 10 + ni;
                    }
                }
                trans_cnt[state] = cnt;
            }
        }
        initialized = 1;
    }

    /* DP arrays for current and next row-pair counts. */
    static seq_t dp_cur[100];
    static seq_t dp_next[100];

    /* For two rows: every ordered pair is allowed. */
    for (int i = 0; i < 100; ++i) dp_cur[i] = (seq_t)1;

    /* Build up to row n by iterating transitions. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 100; ++i) dp_next[i] = (seq_t)0;
        for (int s = 0; s < 100; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            int cnt = trans_cnt[s];
            for (int j = 0; j < cnt; ++j) {
                int t = trans_to[s][j];
                dp_next[t] += val;
            }
        }
        /* Prepare for next step. */
        for (int i = 0; i < 100; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over all ending row pairs. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 100; ++i) result += dp_cur[i];
    return result;
}
