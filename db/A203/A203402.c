/*
 * A203402  Number of (n+2)X5 binary arrays avoiding patterns 000 and 111 in
 *          rows and columns.
 * https://oeis.org/A203402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming counting (n+2)×5 binary arrays
   avoiding horizontal and vertical patterns 000 and 111 (OEIS A203402). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 – we count matrices of size (n+2) × 5. */
    idx_t rows = n + 2;                 /* total number of rows */

    /* ----------------------------------------------------------
       Pre‑compute all 5‑bit row patterns that avoid 000/111
       horizontally, and pre‑compute vertical compatibility
       among triples of rows.  The work is performed only once.
       ---------------------------------------------------------- */
    static int masks[16];              /* actual 5‑bit masks (0…31) */
    static int V = 0;                  /* number of valid rows (≤16) */
    static uint8_t compat[16][16][16]; /* true if three rows a,b,c are
                                         compatible vertically          */
    static int init = 0;
    if (!init) {
        /* Find all valid rows (no horizontal 000 or 111). */
        for (int m = 0; m < 32; ++m) {
            int ok = 1;
            for (int i = 0; i <= 2; ++i) {          /* 5‑bit → 3 starting positions */
                int sub = (m >> i) & 7;            /* 3 consecutive bits */
                if (sub == 0 || sub == 7) {ok = 0; break;}
            }
            if (ok) masks[V++] = m;
        }

        /* Pre‑compute vertical compatibility for all triples of rows. */
        for (int a = 0; a < V; ++a)
            for (int b = 0; b < V; ++b)
                for (int c = 0; c < V; ++c) {
                    uint32_t pa = (uint32_t)masks[a];
                    uint32_t pb = (uint32_t)masks[b];
                    uint32_t pc = (uint32_t)masks[c];

                    /* columns that are all 0 or all 1 in the triple */
                    uint32_t all_zero = (~pa & ~pb & ~pc) & 31u;
                    uint32_t all_one  = (pa & pb & pc)   & 31u;
                    compat[a][b][c] = ! (all_zero | all_one);
                }

        init = 1;
    }

    /* ----------------------------------------------------------
       Special cases: very few rows – no vertical restriction yet
       ---------------------------------------------------------- */
    if (rows == 1)          return (seq_t)V;               /* only one row */
    if (rows == 2)          return (seq_t)V * (seq_t)V;   /* two rows, all pairs allowed */

    /* ----------------------------------------------------------
       DP over rows.
       dp[a][b] – number of ways after “rows_done” rows,
       where the last two rows are a (row index) and b.
       ---------------------------------------------------------- */
    static seq_t dp[16][16];
    static seq_t nxt[16][16];

    /* After fixing the first two rows all pairs are allowed. */
    for (int a = 0; a < V; ++a)
        for (int b = 0; b < V; ++b)
            dp[a][b] = 1;

    idx_t rows_done = 2;
    while (rows_done < rows) {
        /* reset next layer to zero */
        for (int a = 0; a < V; ++a)
            for (int b = 0; b < V; ++b)
                nxt[a][b] = 0;

        /* transition */
        for (int a = 0; a < V; ++a)
            for (int b = 0; b < V; ++b) {
                seq_t cnt = dp[a][b];
                if (!cnt) continue;
                for (int c = 0; c < V; ++c)
                    if (compat[a][b][c])
                        nxt[b][c] += cnt;          /* add new row “c” */
            }

        /* copy back */
        for (int a = 0; a < V; ++a)
            for (int b = 0; b < V; ++b)
                dp[a][b] = nxt[a][b];

        ++rows_done;
    }

    /* Sum over all final states (last two rows). */
    seq_t ans = 0;
    for (int a = 0; a < V; ++a)
        for (int b = 0; b < V; ++b)
            ans += dp[a][b];

    return ans;
}
