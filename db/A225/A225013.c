/*
 * A225013  Number of 6Xn 0..1 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A225013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 28, u64 n <= 207, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A225013: Number of 6×n 0..1 arrays with rows unimodal and columns nondecreasing.
 *
 * This implementation uses dynamic programming over 3^6 = 729 states.
 * A state encodes, for each of the 6 rows, whether we are
 *     0 – before the block of 1's,
 *     1 – inside the block of 1's,
 *     2 – after the block of 1's.
 * A column is specified by a cut value c∈{0,…,6}.  
 * For each row i the bit in that column is `1` iff `c < i`; otherwise it is `0`.
 * The transition from a state `s` to the next state `t` is valid iff no row
 * changes from state 2 (after) to state 1 (inside), i.e. we never re‑enter a
 * block of 1's after it has ended.
 *
 * For each state we precompute the resulting state for every cut value.
 * The DP iterates over the n columns, updating a count of sequences
 * that lead to each state.  The final answer is the sum of all state counts.
 *
 * The algorithm runs in O(n · 729 · 7) time and uses O(729) memory,
 * easily fast enough for n up to 10 000.
 */

#include <stdbool.h>

#define STATE_COUNT 729          /* 3^6 */
static const int POW3[6] = {1, 3, 9, 27, 81, 243};

/* transition[s][c] = new state after reading a column with cut c from state s;
 * -1 indicates an invalid transition.  The table is built once. */
static int transition_table[STATE_COUNT][7] = {0};
static bool transition_inited = false;

static void init_transition_table(void)
{
    for (int s = 0; s < STATE_COUNT; ++s)
    {
        for (int c = 0; c <= 6; ++c)
        {
            int tmp = s;
            int new_state = 0;
            bool valid = true;
            for (int i = 0; i < 6; ++i)
            {
                int s_i = tmp % 3;
                tmp /= 3;
                int bit = (c < i + 1) ? 1 : 0;
                int new_s_i;
                if (s_i == 0)
                {
                    new_s_i = (bit == 0) ? 0 : 1;
                }
                else if (s_i == 1)
                {
                    new_s_i = (bit == 1) ? 1 : 2;
                }
                else /* s_i == 2 */
                {
                    if (bit == 1)
                    {
                        valid = false;
                        break;              /* re‑entering a 1‑block is forbidden */
                    }
                    new_s_i = 2;
                }
                new_state += new_s_i * POW3[i];
            }
            transition_table[s][c] = valid ? new_state : -1;
        }
    }
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* initialise the transition table only once */
    if (!transition_inited)
    {
        init_transition_table();
        transition_inited = true;
    }

    /* dp arrays: counts of ways to reach each state after the processed columns */
    seq_t dp[STATE_COUNT] = {0};
    seq_t next[STATE_COUNT] = {0};

    dp[0] = (seq_t)1;          /* start with all rows before any 1-block */

    for (idx_t col = 0; col < n; ++col)
    {
        /* zero next array */
        for (int i = 0; i < STATE_COUNT; ++i)
            next[i] = (seq_t)0;

        /* transition */
        for (int s = 0; s < STATE_COUNT; ++s)
        {
            seq_t ways = dp[s];
            if (ways == (seq_t)0)
                continue;

            for (int c = 0; c <= 6; ++c)
            {
                int t = transition_table[s][c];
                if (t != -1)
                    next[t] += ways;
            }
        }

        /* swap dp and next */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[i] = next[i];
    }

    /* sum all reachable states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp[i];

    return total;
}
