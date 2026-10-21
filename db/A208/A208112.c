/*
 * A208112  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n 0..1 arrays avoiding * horizontal patterns
 *            0 0 0 and 0 1 0, * and vertical patterns 0 1 1 and 1 1 0. *
 *            Computed via a transfer matrix over column states. * Initial
 *            values: a(1)=35, a(2)=1225
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* static tables are built only once */
    static bool initialized = false;
    static int valcnt;
    static int valid[35];                    /* masks of valid columns */
    static int trans[35][35][35];            /* transition lists k for (i,j) */
    static int trans_cnt[35][35];            /* number of entries in trans[i][j] */

    if (!initialized) {
        /* Determine all column masks that avoid the vertical patterns
           0 1 1 and 1 1 0. */
        valcnt = 0;
        for (int mask = 0; mask < 128; ++mask) {
            bool ok = true;
            for (int r = 0; r <= 4; ++r) {
                int b1 = (mask >> r) & 1;
                int b2 = (mask >> (r + 1)) & 1;
                int b3 = (mask >> (r + 2)) & 1;
                if ((b1 == 0 && b2 == 1 && b3 == 1) ||
                    (b1 == 1 && b2 == 1 && b3 == 0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) valid[valcnt++] = mask;
        }

        /* Pre‑compute horizontal compatibility of triples of columns.
           For each ordered pair (i,j) of valid columns we list all k such
           that (i,j,k) satisfies for every row r:
             not (b[i,r]==0 && b[j,r]==0 && b[k,r]==0)
             not (b[i,r]==0 && b[j,r]==1 && b[k,r]==0)        */
        for (int i = 0; i < valcnt; ++i) {
            for (int j = 0; j < valcnt; ++j) {
                trans_cnt[i][j] = 0;
                for (int k = 0; k < valcnt; ++k) {
                    bool ok = true;
                    for (int r = 0; r < 7; ++r) {
                        int b1 = (valid[i] >> r) & 1;
                        int b2 = (valid[j] >> r) & 1;
                        int b3 = (valid[k] >> r) & 1;
                        if ((b1 == 0 && b2 == 0 && b3 == 0) ||
                            (b1 == 0 && b2 == 1 && b3 == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        trans[i][j][trans_cnt[i][j]++] = k;
                    }
                }
            }
        }
        initialized = true;
    }

    if (n == 1) return (seq_t)valcnt;
    if (n == 2) return (seq_t)valcnt * (seq_t)valcnt;

    /* Dynamic programming over column positions. */
    static seq_t dp[35][35];
    static seq_t ndp[35][35];

    /* Initially, after two columns, every pair of valid columns is possible. */
    for (int i = 0; i < valcnt; ++i)
        for (int j = 0; j < valcnt; ++j)
            dp[i][j] = 1;

    for (idx_t col = 3; col <= n; ++col) {
        for (int i = 0; i < valcnt; ++i)
            for (int j = 0; j < valcnt; ++j)
                ndp[i][j] = 0;

        for (int i = 0; i < valcnt; ++i) {
            for (int j = 0; j < valcnt; ++j) {
                seq_t cur = dp[i][j];
                if (!cur) continue;
                int cnt = trans_cnt[i][j];
                for (int t = 0; t < cnt; ++t) {
                    int k = trans[i][j][t];
                    ndp[j][k] += cur;
                }
            }
        }

        /* copy ndp back to dp for next iteration */
        for (int i = 0; i < valcnt; ++i)
            for (int j = 0; j < valcnt; ++j)
                dp[i][j] = ndp[i][j];
    }

    seq_t total = 0;
    for (int i = 0; i < valcnt; ++i)
        for (int j = 0; j < valcnt; ++j)
            total += dp[i][j];

    return total;
}
