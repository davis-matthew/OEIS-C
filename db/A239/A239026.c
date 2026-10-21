/*
 * A239026  Number of nX4 0..2 arrays with no element equal to the sum of
 *          elements to its left or one plus the sum of elements above it,
 *          modulo 3.
 * https://oeis.org/A239026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries in {0,1,2} such that
 *            for every cell the value is not equal to the sum of elements to
 *            its left (in the same row modulo 3) nor to one plus the sum of
 *            elements above it (in the same column modulo 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define STATE_COUNT 81          /* 3^4 possible column‑sum states */
#define MAX_TRANS 81            /* upper bound on outgoing transitions per state */

/* Precomputed transition tables */
static bool transitions_initialized = false;
static int  trans_to[STATE_COUNT][MAX_TRANS];
static uint8_t trans_count[STATE_COUNT][MAX_TRANS];
static int  trans_n[STATE_COUNT] = {0};

/* Helper to compute transitions only once */
static void init_transitions(void)
{
    if (transitions_initialized) return;
    int count_local[STATE_COUNT];               /* temporary counter for each target state */

    for (int s = 0; s < STATE_COUNT; ++s)
    {
        /* Decode state s into column sums col0..col3 */
        int col0 = s / 27;
        int col1 = (s % 27) / 9;
        int col2 = (s % 9) / 3;
        int col3 = s % 3;

        /* reset local counters for each possible new state */
        for (int t = 0; t < STATE_COUNT; ++t) count_local[t] = 0;

        /* Enumerate all 4‑tuples (v0,v1,v2,v3) satisfying the constraints */
        for (int v0 = 0; v0 < 3; ++v0)
        {
            /* left sum for the first cell is always 0 */
            if (v0 == 0) continue;                           /* cannot equal left sum 0 */
            if (v0 == (col0 + 1) % 3) continue;             /* cannot equal (col0+1) mod 3 */

            int left1 = v0;        /* left sum after first cell */

            for (int v1 = 0; v1 < 3; ++v1)
            {
                if (v1 == left1) continue;                  /* left constraint */
                if (v1 == (col1 + 1) % 3) continue;          /* column constraint */

                int left2 = (left1 + v1) % 3;

                for (int v2 = 0; v2 < 3; ++v2)
                {
                    if (v2 == left2) continue;
                    if (v2 == (col2 + 1) % 3) continue;

                    int left3 = (left2 + v2) % 3;

                    for (int v3 = 0; v3 < 3; ++v3)
                    {
                        if (v3 == left3) continue;
                        if (v3 == (col3 + 1) % 3) continue;

                        /* New column sums after this row */
                        int new0 = (col0 + v0) % 3;
                        int new1 = (col1 + v1) % 3;
                        int new2 = (col2 + v2) % 3;
                        int new3 = (col3 + v3) % 3;
                        int sp = new0 * 27 + new1 * 9 + new2 * 3 + new3;
                        ++count_local[sp];
                    }
                }
            }
        }

        /* Convert local counts to adjacency lists */
        int idx = 0;
        for (int sp = 0; sp < STATE_COUNT; ++sp)
        {
            if (count_local[sp] == 0) continue;
            trans_to[s][idx]   = sp;
            trans_count[s][idx] = (uint8_t)count_local[sp];
            ++idx;
        }
        trans_n[s] = idx;
    }
    transitions_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                     /* undefined for n < 1 */

    init_transitions();

    seq_t dp_cur[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];

    /* initialise DP: all column sums initially 0 */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = (seq_t)0;
    dp_cur[0] = (seq_t)1;

    /* iterate over rows */
    for (idx_t r = 0; r < n; ++r)
    {
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s)
        {
            seq_t cur = dp_cur[s];
            if (cur == (seq_t)0) continue;

            int tcnt = trans_n[s];
            for (int t = 0; t < tcnt; ++t)
            {
                int sp = trans_to[s][t];
                seq_t add = cur * (seq_t)trans_count[s][t];
                dp_next[sp] += add;
            }
        }

        /* copy back */
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum over all final column‑sum states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i) result += dp_cur[i];
    return result;
}
