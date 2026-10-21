/*
 * A238772  Number of nX5 0..3 arrays with no element equal to the sum modulo
 *          4 of elements to its left or elements above it.
 * https://oeis.org/A238772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of n×5 arrays with entries 0..3 such that
   for every cell the entry is not equal to the sum modulo 4 of
   elements to its left or of elements above it.
   The function accepts an index n>=1 and returns a(n). */

#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 1024          /* 4^5 possible column‑sum modulo 4 states */
#define MAX_TRANS   1024          /* upper bound on transitions per state */

/* Build all transitions from a given state.
   state : index in [0,STATE_COUNT) representing the current column sums.
   row   : array to receive target state indices.
   cnt   : pointer to number of transitions appended. */
static void build_row_transition(int state, uint16_t *row, uint16_t *cnt)
{
    uint8_t S[5];
    int tmp = state;
    for (int i = 0; i < 5; ++i) { S[i] = tmp & 3; tmp >>= 2; }

    *cnt = 0;
    /* Enumerate all 5‑tuples of digits 0..3 (total 4^5 = 1024). */
    for (int val = 0; val < STATE_COUNT; ++val) {
        int t = val;
        int d[5];
        for (int c = 0; c < 5; ++c) { d[c] = t & 3; t >>= 2; }

        int left = 0;          /* running sum of current row to the left */
        bool ok = true;
        int newS[5];
        for (int c = 0; c < 5; ++c) {
            int v = d[c];
            if (v == left || v == S[c]) { ok = false; break; }
            newS[c] = (S[c] + v) & 3;
            left = (left + v) & 3;
        }
        if (ok) {
            int newState =
                (newS[0] |
                 (newS[1] << 2) |
                 (newS[2] << 4) |
                 (newS[3] << 6) |
                 (newS[4] << 8));
            row[(*cnt)++] = (uint16_t)newState;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static int init = 0;
    static uint16_t trans[STATE_COUNT][MAX_TRANS];
    static uint16_t trans_cnt[STATE_COUNT];
    static seq_t dp[STATE_COUNT], dp_next[STATE_COUNT];

    if (!init) {
        /* Construct transition table once. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            build_row_transition(s, trans[s], &trans_cnt[s]);
        }
        init = 1;
    }

    /* Initialise DP: all column sums are 0 before any rows. */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Process each of the n rows. */
    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            uint16_t *next = trans[s];
            uint16_t c = trans_cnt[s];
            for (uint16_t k = 0; k < c; ++k) {
                dp_next[next[k]] += val;
            }
        }
        /* Move to the next row. */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    /* Sum all reachable states: the total number of arrays. */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp[i];
    return ans;
}
