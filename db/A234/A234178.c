/*
 * A234178  Number of (n+1)X(2+1) 0..5 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234178: number of (n+1)×3 arrays over {0..5} whose every 2×2 subblock
 * has all edge and diagonal differences at most 1 in absolute value.
 *
 * The DP enumerates the 44 admissible rows (sequences of length 3 with
 * adjacent values differing by at most 1).  Two rows are adjacent if the
 * corresponding 2×2 block satisfies the difference conditions.  The
 * number of arrays of height n+1 equals the number of walks of length n
 * in this graph, which we compute in O(n·44²) time and O(44) memory.
 */

#include <stdlib.h>    /* for abs() */
#include <stdint.h>    /* for standard integer types */

/* The types seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* behavior below offset is irrelevant */

    /*------------------------- Enumerate admissible rows -----------------*/
    static int  rows[60][3];
    static int  rows_sz = 0;
    static int  neigh[60][60];
    static int  neigh_cnt[60] = {0};

    if (rows_sz == 0) {
        /* generate all 44 rows (a,b,c) with |a-b|≤1 and |b-c|≤1 */
        for (int a = 0; a <= 5; ++a) {
            for (int b = a - 1; b <= a + 1; ++b) {
                if (b < 0 || b > 5) continue;
                for (int c = b - 1; c <= b + 1; ++c) {
                    if (c < 0 || c > 5) continue;
                    rows[rows_sz][0] = a;
                    rows[rows_sz][1] = b;
                    rows[rows_sz][2] = c;
                    ++rows_sz;
                }
            }
        }
        /* build adjacency lists for the rows */
        for (int i = 0; i < rows_sz; ++i) {
            for (int j = 0; j < rows_sz; ++j) {
                int ok = 1;
                /* vertical edges */
                for (int k = 0; k <= 2; ++k) {
                    if (abs(rows[i][k] - rows[j][k]) > 1) { ok = 0; break; }
                }
                if (!ok) continue;
                /* diagonal edges (only j=0,1 needed) */
                for (int k = 0; k <= 1; ++k) {
                    if (abs(rows[i][k] - rows[j][k + 1]) > 1) { ok = 0; break; }
                    if (abs(rows[j][k] - rows[i][k + 1]) > 1) { ok = 0; break; }
                }
                if (ok) {
                    neigh[i][neigh_cnt[i]++] = j;
                }
            }
        }
    }

    /*------------------------------ DP ------------------------------------------------*/
    static seq_t dp[60];
    static seq_t next[60];

    /* start with one array of each admissible row (height = 1) */
    for (int i = 0; i < rows_sz; ++i)
        dp[i] = (seq_t)1;

    /* perform n transitions: each adds a new row to the array */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < rows_sz; ++i)
            next[i] = (seq_t)0;
        for (int i = 0; i < rows_sz; ++i) {
            seq_t cur = dp[i];
            if (cur == (seq_t)0) continue;
            int deg = neigh_cnt[i];
            for (int p = 0; p < deg; ++p) {
                int j = neigh[i][p];
                next[j] += cur;
            }
        }
        for (int i = 0; i < rows_sz; ++i)
            dp[i] = next[i];
    }

    /* accumulate total number of arrays of height n+1 */
    seq_t result = (seq_t)0;
    for (int i = 0; i < rows_sz; ++i)
        result += dp[i];

    return result;
}
