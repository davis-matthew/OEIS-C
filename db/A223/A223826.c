/*
 * A223826  Number of nX3 0..2 arrays with rows and columns unimodal.
 * https://oeis.org/A223826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 97, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A223826: Number of n×3 arrays with rows and columns unimodal.
   Computed by dynamic programming over rows: for each column we track the
   last value (0..2) and whether we are still in the non‑decreasing part
   of the column. A valley pattern a > b < c is forbidden in any row. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {          /* out of range, but return 0 */
        return (seq_t)0;
    }

    /* 216 composite states (3 columns, 6 possibilities each). */
    const int MAXSTATE = 216;

    /* Decode information for each state (once). */
    static unsigned char last_val[216][3];
    static unsigned char inc_flag[216][3];
    static bool state_initialized = false;

    if (!state_initialized) {
        for (int st = 0; st < MAXSTATE; ++st) {
            int x = st;
            for (int col = 0; col < 3; ++col) {
                int code = x % 6;          /* 0..5 == (last*2 + inc)   */
                x /= 6;
                last_val[st][col] = (unsigned char)(code >> 1);  /* last = code/2  */
                inc_flag[st][col] = (unsigned char)(code & 1);   /* inc  = code%2  */
            }
        }
        state_initialized = true;
    }

    seq_t dp[MAXSTATE];
    seq_t ndp[MAXSTATE];

    /* Initialise dp to zero. */
    for (int i = 0; i < MAXSTATE; ++i) dp[i] = (seq_t)0;

    /* First row: enumerate all 27 triples, rejecting valley. */
    for (int v0 = 0; v0 <= 2; ++v0)
        for (int v1 = 0; v1 <= 2; ++v1)
            for (int v2 = 0; v2 <= 2; ++v2) {
                if (v0 > v1 && v1 < v2) continue;           /* valley not allowed */

                int code0 = v0 * 2 + 1;                     /* inc = 1 initially */
                int code1 = v1 * 2 + 1;
                int code2 = v2 * 2 + 1;
                int state = code0 + 6 * code1 + 36 * code2;
                dp[state] = dp[state] + (seq_t)1;
            }

    /* Process remaining rows (row 2 .. n). */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < MAXSTATE; ++i) ndp[i] = (seq_t)0;

        for (int st = 0; st < MAXSTATE; ++st) {
            if (dp[st] == (seq_t)0) continue;

            seq_t cur = dp[st];
            /* Enumerate all possible new rows. */
            for (int v0 = 0; v0 <= 2; ++v0)
                for (int v1 = 0; v1 <= 2; ++v1)
                    for (int v2 = 0; v2 <= 2; ++v2) {
                        if (v0 > v1 && v1 < v2) continue;   /* valley not allowed */

                        int codes[3];
                        bool ok = true;

                        /* Column 0 */
                        int last0 = last_val[st][0];
                        int inc0  = inc_flag[st][0];
                        int new_inc0;
                        if (inc0) {
                            if (v0 < last0) new_inc0 = 0;
                            else new_inc0 = 1;
                        } else {
                            if (v0 > last0) { ok = false; }
                            else new_inc0 = 0;
                        }
                        if (!ok) continue;
                        codes[0] = v0 * 2 + new_inc0;

                        /* Column 1 */
                        int last1 = last_val[st][1];
                        int inc1  = inc_flag[st][1];
                        int new_inc1;
                        if (inc1) {
                            if (v1 < last1) new_inc1 = 0;
                            else new_inc1 = 1;
                        } else {
                            if (v1 > last1) { ok = false; }
                            else new_inc1 = 0;
                        }
                        if (!ok) continue;
                        codes[1] = v1 * 2 + new_inc1;

                        /* Column 2 */
                        int last2 = last_val[st][2];
                        int inc2  = inc_flag[st][2];
                        int new_inc2;
                        if (inc2) {
                            if (v2 < last2) new_inc2 = 0;
                            else new_inc2 = 1;
                        } else {
                            if (v2 > last2) { ok = false; }
                            else new_inc2 = 0;
                        }
                        if (!ok) continue;
                        codes[2] = v2 * 2 + new_inc2;

                        int new_state = codes[0] + 6 * codes[1] + 36 * codes[2];
                        ndp[new_state] = ndp[new_state] + cur;
                    }
        }

        /* swap dp and ndp */
        for (int i = 0; i < MAXSTATE; ++i) dp[i] = ndp[i];
    }

    /* Sum all counts to obtain the final answer. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < MAXSTATE; ++i) result = result + dp[i];

    return result;
}
