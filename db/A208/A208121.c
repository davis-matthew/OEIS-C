/*
 * A208121  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6 × n binary arrays avoiding the horizontal
 *            patterns 0 0 0 and 0 1 0, and the vertical patterns 0 0 1 and 0
 *            1 1. The implementation uses a transfer‑matrix on the 16 allowed
 *            column patterns
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0‑… style */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The allowed column patterns are the 16 binary strings of length 6
       that avoid a 0 at position r and a 1 at position r+2 for r=0..3.  These
       are the strings whose bits of even and odd positions separately avoid
       the substring "01". */
    static bool initialized = false;
    static int A;                     /* number of allowed columns (16) */
    static int col_mask[16];          /* 6‑bit mask for each allowed column */
    static int next_list[16][16];     /* for each i, list of k such that
                                         (col_mask[i] | col_mask[k]) == 63 */
    static int next_cnt[16];
    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    if (!initialized) {
        /* Build allowed column masks. */
        int idx = 0;
        for (int m = 0; m < 64; ++m) {
            bool ok = true;
            for (int r = 0; r < 4 && ok; ++r) {
                int b0 = (m >> r) & 1;
                int b2 = (m >> (r + 2)) & 1;
                if (b0 == 0 && b2 == 1)
                    ok = false;            /* forbidden vertical pattern */
            }
            if (ok) {
                col_mask[idx++] = m;
            }
        }
        A = idx;                           /* should be 16 */

        /* Precompute transition lists for the transfer matrix. */
        for (int i = 0; i < A; ++i)
            next_cnt[i] = 0;
        for (int i = 0; i < A; ++i) {
            for (int k = 0; k < A; ++k) {
                if ( (col_mask[i] | col_mask[k]) == 63 ) {
                    next_list[i][next_cnt[i]++] = k;
                }
            }
        }

        /* Zero DP tables. */
        for (int i = 0; i < A; ++i)
            for (int j = 0; j < A; ++j) {
                dp[i][j]  = (seq_t)0;
                ndp[i][j] = (seq_t)0;
            }

        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    if (n == 1) return (seq_t)A;           /* single column */
    if (n == 2) return (seq_t)A * (seq_t)A;/* two columns independent */

    /* Initialize DP for length 2: every pair of allowed columns is possible. */
    for (int i = 0; i < A; ++i)
        for (int j = 0; j < A; ++j)
            dp[i][j] = (seq_t)1;

    /* For each subsequent column add transitions. */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero ndp */
        for (int i = 0; i < A; ++i)
            for (int j = 0; j < A; ++j)
                ndp[i][j] = (seq_t)0;

        /* Transition: (i,j) -> (j,k) if columns i and k together cover all rows. */
        for (int i = 0; i < A; ++i) {
            int deg = next_cnt[i];
            for (int j = 0; j < A; ++j) {
                seq_t val = dp[i][j];
                if (val == 0) continue;
                for (int t = 0; t < deg; ++t) {
                    int k = next_list[i][t];
                    ndp[j][k] += val;
                }
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < A; ++i)
            for (int j = 0; j < A; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all states to get the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < A; ++i)
        for (int j = 0; j < A; ++j)
            total += dp[i][j];

    return total;
}
