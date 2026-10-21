/*
 * A236274  Number of (n+1)X(4+1) 0..2 arrays with the upper median unequal to
 *          the lower median in every 2X2 subblock.
 * https://oeis.org/A236274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of (n+1)-by-5 arrays over {0,1,2} with upper median
   unequal to lower median in every 2×2 subblock. */

#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 243   /* 3^5 possible rows */

/* Encode a 5‑tuple of digits 0..2 into an integer 0..242. */
static inline int encode_state(const int v[5]) {
    return v[0] + v[1] * 3 + v[2] * 9 + v[3] * 27 + v[4] * 81;
}

/* Decode a state into its 5 digits. */
static inline void decode_state(int state, int out[5]) {
    out[0] = state % 3;
    out[1] = (state / 3) % 3;
    out[2] = (state / 9) % 3;
    out[3] = (state / 27) % 3;
    out[4] = (state / 81) % 3;
}

/* Precomputed table indicating whether a given 2×2 pattern satisfies
   the median inequality.                                                        */
static bool median_ok[81];
static bool median_ok_initialized = false;

/* For each state s1 (top row), keep a list of all states s2 (bottom row)
   that can follow it.  We store the predecessors for each destination
   state to speed up the DP step.                                               */
static int prevs[STATE_COUNT][STATE_COUNT];
static unsigned char prevcnt[STATE_COUNT];
static bool initialized = false;

/* Build the median_ok table and the reverse adjacency lists once. */
static void init_globals(void)
{
    if (initialized)
        return;

    /* Build median_ok table.                                     */
    for (int idx = 0; idx < 81; ++idx) {
        int digits[4];
        digits[0] = idx % 3;
        digits[1] = (idx / 3) % 3;
        digits[2] = (idx / 9) % 3;
        digits[3] = (idx / 27) % 3;

        /* Sort the four digits to find the two middle values.    */
        for (int i = 1; i < 4; ++i) {
            int key = digits[i];
            int j = i - 1;
            while (j >= 0 && digits[j] > key) {
                digits[j + 1] = digits[j];
                --j;
            }
            digits[j + 1] = key;
        }
        median_ok[idx] = (digits[1] != digits[2]);   /* lower vs upper median */
    }

    /* Decode all states once to avoid repeated work.             */
    int state_vals[STATE_COUNT][5];
    for (int s = 0; s < STATE_COUNT; ++s)
        decode_state(s, state_vals[s]);

    /* For every pair of states, check if they can be adjacent;   */
    /* if so, store s1 as a predecessor of s2.                    */
    for (int s1 = 0; s1 < STATE_COUNT; ++s1) {
        for (int s2 = 0; s2 < STATE_COUNT; ++s2) {
            bool ok = true;
            for (int j = 0; ok && j < 4; ++j) {
                int a = state_vals[s1][j];
                int b = state_vals[s1][j + 1];
                int c = state_vals[s2][j];
                int d = state_vals[s2][j + 1];
                int pat = a + b * 3 + c * 9 + d * 27;
                if (!median_ok[pat]) {
                    ok = false;
                }
            }
            if (ok) {
                unsigned char pos = prevcnt[s2];
                prevs[s2][pos] = s1;
                prevcnt[s2] = pos + 1;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_globals();

    /* DP arrays: dp_cur is the number of ways to reach each state after
       the current number of transitions.  dp_next holds the next step.   */
    seq_t dp_cur[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];
    seq_t one = 1;

    for (int i = 0; i < STATE_COUNT; ++i)
        dp_cur[i] = one;               /* step 0: first row arbitrary */

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = 0;

        for (int dest = 0; dest < STATE_COUNT; ++dest) {
            unsigned char pc = prevcnt[dest];
            for (unsigned char k = 0; k < pc; ++k) {
                int src = prevs[dest][k];
                dp_next[dest] += dp_cur[src];
            }
        }

        /* swap current and next */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_cur[i] = dp_next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp_cur[i];
    return total;
}
