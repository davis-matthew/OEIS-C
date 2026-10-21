/*
 * A208143  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 46, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A208143 – number of 4×n binary arrays avoiding the
    forbidden patterns 001 and 011 horizontally and 001,101 vertically.
    The algorithm uses a transfer–matrix DP over the last two columns.
    States are 4‑bit column pairs (col_{i-2},col_{i-1}) from the
    8 admissible columns.  A new column 'c' is compatible with the
    previous‑previous column 'a' iff every 1‑bit of 'c' is already a 1‑bit of 'a'
    (i.e. (c & ~a)==0).  The vertical patterns are enforced by
    restricting any column to one of the 8 admissible patterns.  See the
    OEIS entry for details.  The DP runs in O(64·8·n). */

#include <stdint.h>

/* Helper: list of admissible 4‑bit columns (vertical constraints only) */
static const uint8_t ALLOWED[8] = {0, 4, 6, 7, 8, 12, 14, 15};

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;      /* not part of the sequence */
    if (n == 1) return (seq_t)8;      /* single column: 8 admissible patterns */
    if (n == 2) return (seq_t)64;     /* 8×8 pairs of admissible columns  */

    /* Static tables are built only once */
    static int idx_from[16][16];
    static int state_a[64];
    static int state_b[64];
    static int trans_next[64][8];
    static uint8_t trans_cnt[64];
    static int init = 0;

    if (!init) {
        /* mark all indices as invalid initially */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                idx_from[a][b] = -1;

        int state = 0;
        /* enumerate all admissible column pairs */
        for (int i = 0; i < 8; ++i) {
            int a = ALLOWED[i];
            for (int j = 0; j < 8; ++j) {
                int b = ALLOWED[j];
                idx_from[a][b] = state;
                state_a[state] = a;
                state_b[state] = b;
                ++state;
            }
        }
        /* build transition lists: from state (a,b) we can go to (b,c)
           if c ⊆ a (i.e. (c & ~a)==0) and c is admissible.  */
        for (int s = 0; s < 64; ++s) {
            int a = state_a[s];
            int b = state_b[s];
            uint8_t cnt = 0;
            for (int k = 0; k < 8; ++k) {
                int c = ALLOWED[k];
                if ((c & (~a)) == 0) {
                    int nxt = idx_from[b][c];
                    if (nxt >= 0)
                        trans_next[s][cnt++] = nxt;
                }
            }
            trans_cnt[s] = cnt;
        }
        init = 1;
    }

    /* DP arrays */
    seq_t dp[64];
    seq_t ndp[64];
    for (int i = 0; i < 64; ++i) dp[i] = (seq_t)0;

    /* initialise with all admissible column pairs for columns 1 and 2 */
    for (int i = 0; i < 8; ++i) {
        int a = ALLOWED[i];
        for (int j = 0; j < 8; ++j) {
            int b = ALLOWED[j];
            dp[idx_from[a][b]] = (seq_t)1;
        }
    }

    /* iterative DP for columns 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 64; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < 64; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            uint8_t cnt = trans_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int nxt = trans_next[s][k];
                ndp[nxt] += val;
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = ndp[i];
    }

    /* sum of all states gives a(n) */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) total += dp[i];
    return total;
}
