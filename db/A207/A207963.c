/*
 * A207963  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: DP over pairs of vertical columns.
   A 5×n 0/1 array is valid iff
      – each column contains no vertical block 001 or 100;
      – for every row the horizontal 3‑column pattern is neither 000 nor 101.
   DP state: the last two columns (p1,p2).  Transitions to a
   new column c are allowed if c is vertically valid and all
   horizontal 3‑column windows (p1,p2,c) satisfy the row rule.
   The number of valid arrays of width n is the sum of dp states
   after processing n columns.  Initial states for n=2 are all
   pairs of vertically valid columns (no horizontal rule yet). */

#include <stdbool.h>
#include <stdint.h>

#define COLS 32                  /* columns fit in 5 bits : 0..31 */
#define STATE (COLS * COLS)      /* 1024 possible (p1,p2) states */

typedef uint8_t col_t;           /* for readability */

typedef struct {
    uint8_t next[COLS];         /* list of admissible next columns */
    uint8_t cnt;                /* number of admissible columns */
} Trans;

static bool verticalValid[COLS];
static Trans transitions[COLS][COLS];
static bool precomputed = false;

/* Precompute which columns are vertically valid and the transitions. */
static void precompute(void)
{
    /* vertical validity: no 001 or 100 in any 3 consecutive rows */
    for (int col = 0; col < COLS; ++col) {
        bool ok = true;
        for (int start = 0; start < 3; ++start) {
            int bits = (col >> start) & 0x7;   /* 3 consecutive bits */
            if (bits == 0x1 || bits == 0x4) { /* 001 or 100 */
                ok = false;
                break;
            }
        }
        verticalValid[col] = ok;
    }

    /* transitions: for every pair (p1,p2) list admissible next column c */
    for (int p1 = 0; p1 < COLS; ++p1) {
        if (!verticalValid[p1]) continue;
        for (int p2 = 0; p2 < COLS; ++p2) {
            if (!verticalValid[p2]) continue;
            Trans *tr = &transitions[p1][p2];
            tr->cnt = 0;
            for (int c = 0; c < COLS; ++c) {
                if (!verticalValid[c]) continue;
                bool ok = true;
                for (int r = 0; r < 5; ++r) {
                    /* horizontal 3‑column window at this row */
                    int val = ((p1 >> r) & 1) |
                              (((p2 >> r) & 1) << 1) |
                              (((c  >> r) & 1) << 2);
                    if (val == 0 || val == 5) {   /* 000 or 101 */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    tr->next[tr->cnt++] = (uint8_t)c;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    if (!precomputed) {
        precompute();
        precomputed = true;
    }

    /* Count vertically valid columns  (for n=1). */
    int vertCount = 0;
    for (int i = 0; i < COLS; ++i)
        if (verticalValid[i]) ++vertCount;

    if (n == 1) return (seq_t)vertCount;

    /* DP arrays */
    static seq_t dp[STATE];
    static seq_t dp_next[STATE];

    /* initialise dp with all pairs of vertically valid columns */
    for (int i = 0; i < STATE; ++i) dp[i] = 0;
    for (int c1 = 0; c1 < COLS; ++c1)
        if (verticalValid[c1])
            for (int c2 = 0; c2 < COLS; ++c2)
                if (verticalValid[c2])
                    dp[c1 * COLS + c2] = (seq_t)1;

    if (n == 2) {
        seq_t ans = 0;
        for (int i = 0; i < STATE; ++i) ans += dp[i];
        return ans;
    }

    /* process columns 3..n */
    for (idx_t col = 3; col <= n; ++col) {
        for (int i = 0; i < STATE; ++i) dp_next[i] = 0;
        for (int idx = 0; idx < STATE; ++idx) {
            seq_t cur = dp[idx];
            if (cur == 0) continue;
            int p1 = idx / COLS;
            int p2 = idx % COLS;
            const Trans *tr = &transitions[p1][p2];
            for (int t = 0; t < tr->cnt; ++t) {
                int new_state = p2 * COLS + tr->next[t];
                dp_next[new_state] += cur;
            }
        }
        for (int i = 0; i < STATE; ++i) dp[i] = dp_next[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < STATE; ++i) ans += dp[i];
    return ans;
}
