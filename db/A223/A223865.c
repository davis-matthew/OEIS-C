/*
 * A223865  Number of 2Xn 0..3 arrays with rows and antidiagonals unimodal and
 *          columns nondecreasing.
 * https://oeis.org/A223865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 159, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A223865: number of 2×n arrays with entries in {0,1,2,3}
   such that each row is unimodal and each column is nondecreasing.
   Computation is performed by a DP over columns, keeping for each row
   the last value and a flag indicating whether we are still in the
   increasing part (inc) or already in the decreasing part (dec).
   The DP state consists of (value1, state1, value2, state2),
   with value∈{0,1,2,3} and state∈{0,1}.  There are 8×8=64
   possible states. The transition from a state with previous
   value v and state s to a new value x is:
        if s==0 (inc):  allowed if x≤v or x>v;
                           new_state = 1 if x<v else 0
        if s==1 (dec):  allowed only if x≤v; new_state=1
   A pair (x,y) is admissible for the next column only if x≤y,
   ensuring the column is nondecreasing.  The initial column consists
   of all pairs (x,y) with x≤y, each with inc=0.
   The total number of arrays is the sum of DP counts after n columns.
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute transitions for a single row state (enc 0..7) */
    static int next_state[8][4];
    static int next_state_initialised = 0;
    if (!next_state_initialised) {
        for (int enc = 0; enc < 8; ++enc) {
            int val  = enc / 2;   /* 0..3 */
            int st   = enc % 2;   /* 0=inc, 1=dec */
            for (int x = 0; x < 4; ++x) {
                if (st == 0) { /* increasing phase */
                    int new_st = (x < val) ? 1 : 0;
                    next_state[enc][x] = x * 2 + new_st;
                } else {         /* decreasing phase */
                    if (x > val) {
                        next_state[enc][x] = -1; /* invalid */
                    } else {
                        next_state[enc][x] = x * 2 + 1; /* stay dec */
                    }
                }
            }
        }
        next_state_initialised = 1;
    }

    /* DP arrays: dp[64] holds counts for current column */
    seq_t dp[64] = {0};

    /* Initialise for the first column (column 1) */
    for (int x = 0; x < 4; ++x) {
        for (int y = x; y < 4; ++y) {     /* y>=x ensures column nondecreasing */
            int enc1 = x * 2;              /* state = 0 (inc) */
            int enc2 = y * 2;              /* state = 0 (inc) */
            int idx  = enc1 * 8 + enc2;
            dp[idx] += 1;
        }
    }

    /* If only one column, sum the DP counts */
    if (n == 1) {
        seq_t ans = 0;
        for (int i = 0; i < 64; ++i) ans += dp[i];
        return ans;
    }

    /* DP over remaining columns */
    seq_t newdp[64];
    for (idx_t col = 2; col <= n; ++col) {
        for (int i = 0; i < 64; ++i) newdp[i] = 0;
        for (int p = 0; p < 64; ++p) {
            if (dp[p] == 0) continue;
            int enc1 = p / 8;   /* state for row1 */
            int enc2 = p % 8;   /* state for row2 */
            for (int x = 0; x < 4; ++x) {
                int ns1 = next_state[enc1][x];
                if (ns1 == -1) continue;          /* invalid transition for row1 */
                for (int y = x; y < 4; ++y) {   /* maintain column nondecreasing */
                    int ns2 = next_state[enc2][y];
                    if (ns2 == -1) continue;      /* invalid transition for row2 */
                    int idx = ns1 * 8 + ns2;
                    newdp[idx] += dp[p];
                }
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = newdp[i];
    }

    /* Sum all final states to obtain the answer */
    seq_t ans = 0;
    for (int i = 0; i < 64; ++i) ans += dp[i];
    return ans;
}
