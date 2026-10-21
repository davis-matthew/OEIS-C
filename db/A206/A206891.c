/*
 * A206891  Number of 5Xn 0..1 arrays avoiding 0 1 0 horizontally and 1 0 0
 *          vertically.
 * https://oeis.org/A206891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays avoiding (a) the horizontal
 *            pattern 0 1 0 in any row, and (b) the vertical pattern 1 0 0 in
 *            any column. a(1) = 20, a(2) = 400, a(3) = 4792,
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* pre‑compute the set of admissible columns and transition tables once */
    static int valid[32];
    static unsigned char trans_len[32][32];
    static int trans[32][32][32];
    static int count_valid = 0;
    static int initialized = 0;

    if (!initialized) {
        /* determine which 5‑bit columns satisfy the vertical rule */
        for (int p = 0; p < 32; ++p) {
            int ok = 1;
            for (int r = 0; r < 3; ++r) {
                int b0 = (p >> r)     & 1;
                int b1 = (p >> (r+1)) & 1;
                int b2 = (p >> (r+2)) & 1;
                if (b0 == 1 && b1 == 0 && b2 == 0) {
                    ok = 0;
                    break;
                }
            }
            valid[p] = ok;
            if (ok) ++count_valid;
        }

        /* precompute transitions (prevprev, prev) -> cur */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                int idx = 0;
                for (int c = 0; c < 32; ++c) {
                    if (!valid[c]) continue;      /* cur column must be valid */
                    int good = 1;
                    for (int r = 0; r < 5; ++r) {
                        int bp0 = (a >> r) & 1;
                        int bp1 = (b >> r) & 1;
                        int bp2 = (c >> r) & 1;
                        if (bp0 == 0 && bp1 == 1 && bp2 == 0) {
                            good = 0;
                            break;
                        }
                    }
                    if (good) {
                        trans[a][b][idx++] = c;
                    }
                }
                trans_len[a][b] = idx;
            }
        }
        initialized = 1;
    }

    if (n == 0) return 0; /* not defined, but safe */

    if (n == 1) {
        return (seq_t)count_valid;
    }

    if (n == 2) {
        return (seq_t)count_valid * (seq_t)count_valid;
    }

    /* DP arrays for two consecutive columns */
    seq_t dp[32][32] = {0};
    seq_t ndp[32][32] = {0};

    /* initialise for first two columns */
    for (int i = 0; i < 32; ++i) if (valid[i]) {
        for (int j = 0; j < 32; ++j) if (valid[j]) {
            dp[i][j] = 1;
        }
    }

    idx_t col;
    for (col = 3; col <= n; ++col) {
        /* zero next DP */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t ways = dp[a][b];
                if (!ways) continue;
                unsigned char len = trans_len[a][b];
                int *list = trans[a][b];
                for (unsigned char k = 0; k < len; ++k) {
                    int c = list[k];
                    ndp[b][c] += ways;
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            total += dp[a][b];

    return total;
}
