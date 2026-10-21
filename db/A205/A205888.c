/*
 * A205888  Number of (n+1)X3 0..2 arrays with 2X2 subblock determinants
 *          differing from horizontal neighbors and permanents differing from
 *          vertical neighbors.
 * https://oeis.org/A205888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 3 arrays with entries 0..2, such that
 *            for each 2×2 subblock the determinant differs from its
 *            horizontal neighbor and the permanent differs from its vertical
 *            neighbor
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    /* Historic constants for one 3‑digit row over {0,1,2} */
    static unsigned char row[27][3];
    /* validity of a pair, determinants must differ */
    static bool pair_valid[27][27];
    /* permanents for each pair, 0‑based column indices 0 (cols 0‑1) and 1 (cols 1‑2) */
    static unsigned char p0[27][27];
    static unsigned char p1[27][27];
    /* pre‑computed adjacency list of valid second rows */
    static unsigned char neigh[27][27];
    static int neigh_cnt[27];
    /* single‑time initialization guard */
    static bool init = false;

    if (!init) {
        init = true;
        /* encode all 27 possible rows */
        for (idx_t i = 0; i < 27; ++i) {
            idx_t tmp = i;
            row[i][0] = (unsigned char)(tmp / 9);
            tmp %= 9;
            row[i][1] = (unsigned char)(tmp / 3);
            tmp %= 3;
            row[i][2] = (unsigned char)(tmp);
        }
        /* pre‑compute pair data */
        for (idx_t i = 0; i < 27; ++i) {
            for (idx_t j = 0; j < 27; ++j) {
                unsigned char a0 = row[i][0], a1 = row[i][1], a2 = row[i][2];
                unsigned char b0 = row[j][0], b1 = row[j][1], b2 = row[j][2];
                int d0 = a0 * b1 - a1 * b0;
                int d1 = a1 * b2 - a2 * b1;
                pair_valid[i][j] = (d0 != d1);
                p0[i][j] = (unsigned char)(a0 * b1 + a1 * b0);
                p1[i][j] = (unsigned char)(a1 * b2 + a2 * b1);
            }
        }
        /* adjacency lists for transitions */
        for (idx_t cur = 0; cur < 27; ++cur) {
            neigh_cnt[cur] = 0;
            for (idx_t nxt = 0; nxt < 27; ++nxt) {
                if (pair_valid[cur][nxt]) {
                    neigh[cur][neigh_cnt[cur]++] = (unsigned char)nxt;
                }
            }
        }
    }

    /* Base case: n == 1 → the array has 2 rows, only the determinant condition applies */
    if (n == 1) {
        seq_t total = 0;
        for (idx_t i = 0; i < 27; ++i)
            for (idx_t j = 0; j < 27; ++j)
                if (pair_valid[i][j])
                    total += 1;
        return total;
    }

    /* DP arrays: dp[prev][cur] holds the count of arrays ending with rows (prev,cur) */
    static seq_t dp[27][27];
    static seq_t nxt[27][27];

    /* initialize dp for the first two rows (n = 1 case) */
    for (idx_t i = 0; i < 27; ++i)
        for (idx_t j = 0; j < 27; ++j)
            dp[i][j] = pair_valid[i][j] ? 1 : 0;

    /* add rows 2 … n (total of n‑1 additional rows) */
    for (idx_t step = 1; step < n; ++step) {
        /* zero the next layer */
        for (idx_t i = 0; i < 27; ++i)
            for (idx_t j = 0; j < 27; ++j)
                nxt[i][j] = 0;

        for (idx_t prev = 0; prev < 27; ++prev) {
            for (idx_t cur = 0; cur < 27; ++cur) {
                seq_t cnt = dp[prev][cur];
                if (!cnt) continue;              /* no arrays reach this state */
                /* permanents of the pair (prev,cur) */
                unsigned char prev_p0 = p0[prev][cur];
                unsigned char prev_p1 = p1[prev][cur];
                /* iterate over possible next rows */
                int ec = neigh_cnt[cur];
                for (int t = 0; t < ec; ++t) {
                    unsigned char nxt_row = neigh[cur][t];
                    if (prev_p0 != p0[cur][nxt_row] &&
                        prev_p1 != p1[cur][nxt_row])
                        nxt[cur][nxt_row] += cnt;
                }
            }
        }
        /* copy nxt into dp for the next iteration */
        for (idx_t i = 0; i < 27; ++i)
            for (idx_t j = 0; j < 27; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* sum all states: last two rows can be arbitrary */
    seq_t total = 0;
    for (idx_t i = 0; i < 27; ++i)
        for (idx_t j = 0; j < 27; ++j)
            total += dp[i][j];

    return total;
}
