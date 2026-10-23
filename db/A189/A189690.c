/*
 * A189690  Number of nX3 binary arrays without the pattern 0 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A189690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×3 binary matrices with no 0 0 1 pattern *
 *            (vertically, diagonally or anti‑diagonally). We compute the *
 *            values by dynamic programming over the rows: each state is the *
 *            last two rows (3 bits each) and we count the number of ways to *
 *            extend them to the next row while avoiding the fo...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Pre‑computed allowed transitions: from a pair of rows (previous·2+next)
 * to all possible next rows. */
static unsigned char allowed_rows[64][8];
static unsigned char allowed_count[64];
static bool transitions_inited = false;

/* Helper: return true iff three entries form the forbidden pattern 0 0 1. */
static inline bool bad_triple(int a, int b, int c)
{
    return a == 0 && b == 0 && c == 1;
}

/* Build the transition table once. */
static void init_transitions(void)
{
    for (int s = 0; s < 64; ++s) {
        int prev2 = s >> 3;      /* high 3 bits  (row i‑2)   */
        int prev1 = s & 7;       /* low  3 bits  (row i‑1)   */
        unsigned char cnt = 0;
        for (int r = 0; r < 8; ++r) {               /* candidate row i */
            /* vertical triples */
            if (bad_triple((prev2 >> 0) & 1, (prev1 >> 0) & 1, (r >> 0) & 1)) continue;
            if (bad_triple((prev2 >> 1) & 1, (prev1 >> 1) & 1, (r >> 1) & 1)) continue;
            if (bad_triple((prev2 >> 2) & 1, (prev1 >> 2) & 1, (r >> 2) & 1)) continue;
            /* main diagonal (row i‑2, row i‑1, row i) */
            if (bad_triple((prev2 >> 0) & 1, (prev1 >> 1) & 1, (r >> 2) & 1)) continue;
            /* anti‑diagonal */
            if (bad_triple((prev2 >> 2) & 1, (prev1 >> 1) & 1, (r >> 0) & 1)) continue;
            allowed_rows[s][cnt++] = (unsigned char)r;
        }
        allowed_count[s] = cnt;
    }
    transitions_inited = true;
}

/* Cache for computed sequence values:  cache[n-1] holds a(n).   */
static seq_t cache[10001];
static idx_t cache_len = 0;   /* number of elements currently stored */

/* DP state for the most recently computed n (dp_cur holds counts
 * for all 64 possible last‑two‑row states).  dp_next holds the new
 * counts for the next n.  We keep the DP arrays fixed in size
 * regardless of n. */
static seq_t dp_cur[64];
static seq_t dp_next[64];
static idx_t current_n = 0;

/* Return a(n) for n >= 1.  Undefined behaviour for n < 1.  */
seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 1.  */
    if (n == 0) return 0;

    /* Initialise cache and DP on first call.  */
    if (!transitions_inited) {
        init_transitions();
        /* Base cases: a(1)=8, a(2)=64.  */
        cache[0] = (seq_t)8;          /* n = 1 */
        cache[1] = (seq_t)64;         /* n = 2 */
        cache_len = 2;

        /* DP for n = 2: there are exactly 1 way to realise any pair of rows
         * (just choose the two rows arbitrarily).  */
        for (int i = 0; i < 64; ++i) dp_cur[i] = (seq_t)1;
        current_n = 2;
    }

    /* If the desired n is already cached, return it. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute values up to n.  */
    for (idx_t step = current_n + 1; step <= n; ++step) {
        /* Clear dp_next. */
        for (int i = 0; i < 64; ++i) dp_next[i] = (seq_t)0;

        /* Transition from dp_cur to dp_next. */
        for (int s = 0; s < 64; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            unsigned char cnt  = allowed_count[s];
            int prev1 = s & 7;    /* the last row of state s */
            for (unsigned char k = 0; k < cnt; ++k) {
                int r  = allowed_rows[s][k];
                int t  = (prev1 << 3) | r;    /* new state for next step */
                dp_next[t] += val;
            }
        }

        /* Sum all dp_next counts to obtain a(step). */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 64; ++i) total += dp_next[i];
        cache[step - 1] = total;

        /* Swap dp_cur and dp_next for the next iteration. */
        for (int i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
        current_n = step;
        cache_len = step;
    }

    return cache[n - 1];
}
