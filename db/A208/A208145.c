/*
 * A208145  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays avoiding the patterns 0 0 1
 *            and 0 1 1 horizontally and 0 0 1 and 1 0 1 vertically. Computed
 *            by a transfer‑matrix DP. (a(1) = 12, a(2) = 144, …)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 173, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute allowed columns and transitions only once. */
    static int  init = 0;
    static int  col_idx[64];
    static int  allowed[64];
    static size_t m = 0;                /* number of allowed columns    */
    static size_t next_cnt[64];
    static int   next_list[64][64];     /* for each a: list of allowed c */
    static seq_t dp_cur[4096];
    static seq_t dp_next[4096];
    size_t i, j, k, s, t;
    idx_t step;

    if (!init)
    {
        /* Mark all columns as not allowed. */
        for (i = 0; i < 64; ++i) col_idx[i] = -1;

        /* Determine which 6‑bit column vectors satisfy the vertical
           restrictions:
           no occurrence of a 0 immediately followed by a 1 in rows
           1→2, 2→3, 3→4, 4→5 (indices start at 0). */
        for (i = 0; i < 64; ++i)
        {
            int ok = 1;
            for (j = 1; j <= 4; ++j)
            {
                int lo = (i >> j)   & 1;  /* bit at row j          */
                int hi = (i >> (j+1)) & 1;/* bit at row j+1        */
                if (lo == 0 && hi == 1) { ok = 0; break; }
            }
            if (ok)
            {
                col_idx[i] = m;
                allowed[m++] = (int)i;
            }
        }

        /* Build transitions: for each allowed column a,
           c is permitted next column iff (a | c) == a
           (i.e., every 1 in c must also be a 1 in a). */
        for (i = 0; i < m; ++i)
        {
            int a = allowed[i];
            next_cnt[i] = 0;
            for (j = 0; j < m; ++j)
            {
                int c = allowed[j];
                if ((a | c) == a)
                {
                    next_list[i][next_cnt[i]++] = (int)j;
                }
            }
        }
        init = 1;
    }

    if (n < 1) return (seq_t)0;

    /* n = 1 : simply the number of allowed columns */
    if (n == 1) return (seq_t)m;

    /* n = 2 : each pair of allowed columns is valid */
    if (n == 2) return (seq_t)m * (seq_t)m;

    /* For n >= 3, DP over pairs of consecutive columns.
       State (a,b) means columns a then b are the last two columns used. */
    size_t state_cnt = m * m;

    /* Initialise dp for length 2: each state has weight 1 */
    for (s = 0; s < state_cnt; ++s) dp_cur[s] = (seq_t)1;

    for (step = 3; step <= n; ++step)
    {
        for (s = 0; s < state_cnt; ++s) dp_next[s] = (seq_t)0;

        for (i = 0; i < m; ++i)          /* previous column a   */
        {
            size_t base = i * m;
            for (j = 0; j < m; ++j)      /* previous column b   */
            {
                seq_t val = dp_cur[base + j];
                if (!val) continue;
                /* next column c determined only by a (middle column j ignored) */
                for (k = 0; k < next_cnt[i]; ++k)
                {
                    t = j * m + next_list[i][k];
                    dp_next[t] += val;
                }
            }
        }
        /* swap dp arrays for next iteration */
        for (s = 0; s < state_cnt; ++s) dp_cur[s] = dp_next[s];
    }

    /* Sum over all states to get total number of arrays of length n */
    seq_t result = 0;
    for (s = 0; s < state_cnt; ++s) result += dp_cur[s];

    return result;
}
