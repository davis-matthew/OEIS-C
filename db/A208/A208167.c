/*
 * A208167  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208167: Number of 6×n binary arrays avoiding
 *  horizontally 0 0 0 and 1 0 1
 *  vertically 0 0 1 and 0 1 0.
 *
 *  Computation is performed by a transfer‑matrix DP.
 *  The state is the last two columns of the array;
 *  there are 21 columns that satisfy the vertical
 *  constraints, so 441 possible states.
 *  Initial values:
 *     a(1) = 21
 *     a(2) = 441
 *  For n ≥ 3 the DP updates along the columns.
 */

#include <stdbool.h>

#define STATE_COLS 21      /* number of valid columns */
#define STATE_COUNT (STATE_COLS * STATE_COLS)  /* 441 */

/* Function prototype expected by compiler */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 21;
    if (n == 2) return 441;

    /* ---------- one‑time initialization of constants ---------- */
    static bool init = false;
    static uint8_t valid_cols[STATE_COLS];
    static uint8_t col_id_map[64];
    static uint8_t transition_count[STATE_COUNT];
    static int transition_next[STATE_COUNT][STATE_COLS]; /* up to 21 next states */

    if (!init)
    {
        /* Find all 6‑bit columns that satisfy vertical constraints */
        int count = 0;
        for (int d = 0; d < 64; d++)
        {
            bool ok = true;
            for (int i = 0; i < 4; i++)
            {
                int b0 = (d >> i)   & 1;
                int b1 = (d >> (i+1)) & 1;
                int b2 = (d >> (i+2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 0 && b1 == 1 && b2 == 0))
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                valid_cols[count] = (uint8_t)d;
                col_id_map[d] = count;
                count++;
            }
        }

        /* Transition table: from (pp,p) to all allowed next columns */
        for (int pp = 0; pp < STATE_COLS; pp++)
        {
            int col_pp = valid_cols[pp];
            for (int p = 0; p < STATE_COLS; p++)
            {
                int col_p = valid_cols[p];
                int state = pp * STATE_COLS + p;
                int cnt = 0;
                for (int nxt = 0; nxt < STATE_COLS; nxt++)
                {
                    int col_n = valid_cols[nxt];
                    bool ok = true;
                    for (int r = 0; r < 6; r++)
                    {
                        int b_pp = (col_pp >> r) & 1;
                        int b_p  = (col_p  >> r) & 1;
                        int b_n  = (col_n  >> r) & 1;
                        if ((b_pp == 0 && b_p == 0 && b_n == 0) ||
                            (b_pp == 1 && b_p == 0 && b_n == 1))
                        {
                            ok = false;
                            break;
                        }
                    }
                    if (ok)
                    {
                        transition_next[state][cnt++] = p * STATE_COLS + nxt;
                    }
                }
                transition_count[state] = (uint8_t)cnt;
            }
        }
        init = true;
    }

    /* ---------- DP arrays ---------- */
    seq_t dp_prev[STATE_COUNT];
    seq_t dp_curr[STATE_COUNT];

    /* initialize DP for first two columns: each pair occurs exactly once */
    for (int i = 0; i < STATE_COUNT; i++) dp_prev[i] = 0;
    for (int pp = 0; pp < STATE_COLS; pp++)
        for (int p = 0; p < STATE_COLS; p++)
            dp_prev[pp * STATE_COLS + p] = 1;

    /* iterate from column 3 up to n */
    for (idx_t col = 3; col <= n; col++)
    {
        for (int i = 0; i < STATE_COUNT; i++) dp_curr[i] = 0;

        for (int state = 0; state < STATE_COUNT; state++)
        {
            seq_t val = dp_prev[state];
            if (val == 0) continue;
            int cnt = transition_count[state];
            for (int k = 0; k < cnt; k++)
            {
                int next_state = transition_next[state][k];
                dp_curr[next_state] += val;
            }
        }

        /* move to next column */
        for (int i = 0; i < STATE_COUNT; i++) dp_prev[i] = dp_curr[i];
    }

    /* sum all states: total number of arrays of length n */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; i++) total += dp_prev[i];
    return total;
}
