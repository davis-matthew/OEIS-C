/*
 * A202587  Number of (n+2)X6 binary arrays avoiding patterns 001 and 100 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP: Count (n+2)×6 binary arrays that avoid the patterns 001 and 100
   in every row, column and northwest–southeast diagonal.  The algorithm
   processes the array row‑by‑row; the state is the last two rows
   (each represented by 6 bits).  For each state we pre‑compute the set
   of allowed next rows and then run a transfer‑matrix style DP.  The
   computation works for arbitrary 64‑bit or bignum seq_t types. */
#include <stdbool.h>
#include <stdint.h>

static bool init_done = false;
static int V;                       /* number of valid 6‑bit rows          */
static int valid_rows[64];          /* pattern value of each valid row     */
static int row_to_idx[64];          /* map pattern -> index (or -1)       */
static uint64_t trans[64][64];      /* allowed next rows for each pair     */

/* ------------------------------------------------------------------ */
/*  Precomputation of valid rows and transition masks                  */
static void init(void)
{
    /* Build list of row patterns that avoid 001 and 100 in the row     */
    V = 0;
    for (int pat = 0; pat < 64; ++pat) {
        bool ok = true;
        for (int c = 0; c <= 3; ++c) {
            int t = (pat >> c) & 7;   /* 3 consecutive bits */
            if (t == 1 || t == 4) {   /* 001 or 100          */
                ok = false;
                break;
            }
        }
        row_to_idx[pat] = -1;
        if (ok) {
            valid_rows[V] = pat;
            row_to_idx[pat] = V;
            ++V;
        }
    }

    /* Precompute transition masks for each pair of last rows          */
    for (int i = 0; i < V; ++i) {
        int p2 = valid_rows[i];
        for (int j = 0; j < V; ++j) {
            int p1 = valid_rows[j];
            uint64_t m = 0;
            for (int k = 0; k < V; ++k) {
                int cur = valid_rows[k];

                /* vertical constraint  (rows r-2, r-1, r)          */
                bool good = true;
                for (int c = 0; c < 6; ++c) {
                    int t = ((p2 >> c) & 1) << 2 |
                            ((p1 >> c) & 1) << 1 |
                            ((cur >> c) & 1);
                    if (t == 1 || t == 4) { good = false; break; }
                }
                if (!good) continue;

                /* diagonal constraint (NW–SE)                       */
                for (int c = 2; c < 6 && good; ++c) {
                    int t = ((p2 >> (c - 2)) & 1) << 2 |
                            ((p1 >> (c - 1)) & 1) << 1 |
                            ((cur >> c) & 1);
                    if (t == 1 || t == 4) good = false;
                }
                if (good) m |= (1ULL << k);
            }
            trans[i][j] = m;
        }
    }
    init_done = true;
}

/* ------------------------------------------------------------------ */
/*  Dynamic programming over the rows                                 */
seq_t a(idx_t n)
{
    if (!init_done) init();

    /* Base cases: at least two rows have been chosen              */
    static seq_t dp[64][64];
    static seq_t dp_next[64][64];

    /* initialise dp with 1 for all pairs (row0, row1)              */
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            dp[i][j] = (seq_t)1;

    /* perform transitions for rows 2 .. n+1 (total n+2 rows)      */
    for (idx_t step = 2; step <= n + 1; ++step) {
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                seq_t ways = dp[i][j];
                if (ways == (seq_t)0) continue;

                uint64_t m = trans[i][j];
                while (m) {
                    int k = __builtin_ctzll(m);
                    dp_next[j][k] += ways;
                    m &= m - 1;
                }
            }
        }
        /* swap dp and dp_next for next iteration                  */
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* sum over all final states (last two rows)                    */
    seq_t result = (seq_t)0;
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            result += dp[i][j];

    return result;
}
