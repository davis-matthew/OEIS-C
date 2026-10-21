/*
 * A207444  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of 4×n binary matrices that avoid the
   submatrices 0 0 1 and 0 1 0 horizontally and 0 0 1 and 1 0 0 vertically.
   The counting is performed by a DP over the 9 admissible column patterns,
   keeping track of the last two columns (81 states).  The algorithm runs
   in O(n) time. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base cases – for completeness and speed */
    if (n == 0) return 0;           /* offset is 1, value for n=0 unused */
    if (n == 1) return 9;
    if (n == 2) return 81;

    /* ------------------------------------------------------------------
     * 1.  Pre‑compute the 9 admissible 4‑bit columns (no vertical 001/100)
     *    and the transition table between states (two previous columns).
     * ------------------------------------------------------------------ */
    enum { M = 9, STATE_CNT = M * M };

    static bool initialized = false;

    static int  valid[M];               /* the 4‑bit integer [0,15] */
    static int  bits[M][4];             /* bits[row] for each column */
    static int  nextList[STATE_CNT][M]; /* allowed next columns for each state */
    static int  nextCount[STATE_CNT];    /* number of allowed next columns */

    if (!initialized)
    {
        /* Collect all columns that avoid vertical 001 and 100. */
        int idx = 0;
        for (int col = 0; col < 16; ++col)
        {
            int b0 = (col >> 0) & 1;
            int b1 = (col >> 1) & 1;
            int b2 = (col >> 2) & 1;
            int b3 = (col >> 3) & 1;

            /* Two vertical triples: rows 0‑2 and rows 1‑3. */
            int pat1 = (b0 << 2) | (b1 << 1) | b2;   /* rows 0‑2 */
            int pat2 = (b1 << 2) | (b2 << 1) | b3;   /* rows 1‑3 */

            if (pat1 == 1 || pat1 == 4 || pat2 == 1 || pat2 == 4)
                continue;        /* forbidden vertical pattern */

            valid[idx]   = col;
            bits[idx][0] = b0;
            bits[idx][1] = b1;
            bits[idx][2] = b2;
            bits[idx][3] = b3;
            ++idx;
        }

        /* Build transition table: for every pair (prevprev, prev)
           determine all admissible next columns cur. */
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < M; ++j)
            {
                int state = i * M + j;
                int cnt   = 0;

                for (int k = 0; k < M; ++k)
                {
                    bool ok = true;
                    for (int r = 0; r < 4 && ok; ++r)
                    {
                        int hor = (bits[i][r] << 2) | (bits[j][r] << 1) | bits[k][r];
                        if (hor == 1 || hor == 2)   /* 001 or 010 forbidden */
                            ok = false;
                    }
                    if (ok)
                        nextList[state][cnt++] = k;
                }
                nextCount[state] = cnt;
            }
        }

        initialized = true;
    }

    /* ------------------------------------------------------------------
     * 2.  Dynamic programming over the columns.
     * ------------------------------------------------------------------ */
    static seq_t dp[STATE_CNT];
    static seq_t newdp[STATE_CNT];

    /* For the first two columns, every ordered pair of admissible columns
       is possible – initialise the dp table accordingly. */
    for (int s = 0; s < STATE_CNT; ++s)
        dp[s] = 1;

    /* Add columns 3 … n.  For each new column we apply the transition
       table that already enforces both vertical and horizontal rules. */
    for (idx_t step = 3; step <= n; ++step)
    {
        for (int s = 0; s < STATE_CNT; ++s)
            newdp[s] = 0;

        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < M; ++j)
            {
                int state = i * M + j;
                seq_t cur = dp[state];
                if (cur == 0) continue;

                int cnt = nextCount[state];
                for (int t = 0; t < cnt; ++t)
                {
                    int k        = nextList[state][t];
                    int newstate = j * M + k;
                    newdp[newstate] += cur;
                }
            }
        }

        /* Swap dp and newdp for the next step. */
        for (int s = 0; s < STATE_CNT; ++s)
            dp[s] = newdp[s];
    }

    /* Sum over all ending states to obtain the total number of boards. */
    seq_t total = 0;
    for (int s = 0; s < STATE_CNT; ++s)
        total += dp[s];

    return total;
}
