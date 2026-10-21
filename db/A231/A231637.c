/*
 * A231637  Number of n X 3 0..2 arrays with no element having a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors equal to itself plus one mod 3, with upper left element
 *          zero (rock paper and scissors drawn positions).
 * https://oeis.org/A231637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} with no element having
 *            a strict majority of its horizontal, vertical, diagonal and
 *            antidiagonal neighbors equal to itself+1 mod 3, with the
 *            upper‑left element fixed to 0. Computed by dynamic programming
 *            over rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*--- helper --------------------------------------------------------*/
/* Test whether 'current' row satisfies the rule given the rows 'above'
   and 'below' (use -1 in the arrays if a neighbour does not exist). */
static inline bool row_valid(const int *above,
                             const int *current,
                             const int *below)
{
    for (int j = 0; j < 3; ++j) {
        int cur   = current[j];
        int succ  = (cur + 1) % 3;
        int tot   = 0;
        int good  = 0;

        /* horizontal neighbours */
        if (j > 0) { ++tot; if (current[j - 1] == succ) ++good; }
        if (j < 2) { ++tot; if (current[j + 1] == succ) ++good; }

        /* vertical neighbours */
        if (above[j] != -1)     { ++tot; if (above[j]  == succ) ++good; }
        if (below[j] != -1)     { ++tot; if (below[j] == succ) ++good; }

        /* diagonal neighbours */
        if (j > 0 && above[j - 1] != -1) {
            ++tot; if (above[j - 1] == succ) ++good;
        }
        if (j < 2 && above[j + 1] != -1) {
            ++tot; if (above[j + 1] == succ) ++good;
        }
        if (j > 0 && below[j - 1] != -1) {
            ++tot; if (below[j - 1] == succ) ++good;
        }
        if (j < 2 && below[j + 1] != -1) {
            ++tot; if (below[j + 1] == succ) ++good;
        }

        if (2 * good > tot)        /* strict majority exists */
            return false;
    }
    return true;
}

/*--- main function ------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 27 possible rows, each stored as 3 digits in base 3 */
    static int rows[27][3];

    /* static tables, built once */
    static bool initialized = false;
    static unsigned char next_cnt[27][27];
    static unsigned char next_list[27][27][27];
    static bool final_ok[27][27];

    const int dummy[3] = { -1, -1, -1 };

    if (!initialized) {
        /* enumerate all rows */
        for (int idx = 0; idx < 27; ++idx) {
            int v = idx;
            rows[idx][0] = v % 3;
            rows[idx][1] = (v / 3) % 3;
            rows[idx][2] = (v / 9) % 3;
        }

        /* build transition tables: for each pair (a,b) list all c making b valid
           with a above and c below */
        for (int a = 0; a < 27; ++a) {
            for (int b = 0; b < 27; ++b) {
                unsigned char cnt = 0;
                for (int c = 0; c < 27; ++c) {
                    if (row_valid(rows[a], rows[b], rows[c])) {
                        next_list[a][b][cnt] = (unsigned char)c;
                        ++cnt;
                    }
                }
                next_cnt[a][b] = cnt;
            }
        }

        /* final validation for the last row (no below neighbours) */
        for (int a = 0; a < 27; ++a) {
            for (int b = 0; b < 27; ++b) {
                final_ok[a][b] = row_valid(rows[a], rows[b], dummy);
            }
        }

        initialized = true;
    }

    /* n == 1 : only the first row exists */
    if (n == 1) {
        seq_t count = 0;
        for (int r = 0; r < 27; ++r) {
            if (rows[r][0] != 0) continue;                /* upper‑left must be 0 */
            if (row_valid(dummy, rows[r], dummy))
                count += (seq_t)1;
        }
        return count;
    }

    /* DP arrays for pairs (prev, cur) – static to avoid repeated allocations */
    static seq_t dp[27*27];
    static seq_t newdp[27*27];
    for (int i = 0; i < 27*27; ++i) dp[i] = 0;

    /* initialise with the first two rows */
    for (int r0 = 0; r0 < 27; ++r0) {
        if (rows[r0][0] != 0) continue;          /* upper‑left constraint */
        for (int r1 = 0; r1 < 27; ++r1) {
            if (row_valid(dummy, rows[r0], rows[r1]))
                dp[r0 * 27 + r1] = (seq_t)1;
        }
    }

    /* process rows 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 27*27; ++i) newdp[i] = 0;
        for (int prev = 0; prev < 27; ++prev) {
            for (int cur = 0; cur < 27; ++cur) {
                seq_t val = dp[prev * 27 + cur];
                if (!val) continue;
                unsigned char cnt = next_cnt[prev][cur];
                for (unsigned char k = 0; k < cnt; ++k) {
                    int nxt = next_list[prev][cur][k];
                    newdp[cur * 27 + nxt] += val;
                }
            }
        }
        for (int i = 0; i < 27*27; ++i) dp[i] = newdp[i];
    }

    /* final sum over states where the last row satisfies the rule */
    seq_t ans = 0;
    for (int prev = 0; prev < 27; ++prev) {
        for (int cur = 0; cur < 27; ++cur) {
            if (final_ok[prev][cur])
                ans += dp[prev * 27 + cur];
        }
    }
    return ans;
}
