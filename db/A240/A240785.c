/*
 * A240785  Number of 4Xn 0..1 arrays with no element equal to fewer vertical
 *          neighbors than horizontal neighbors, with new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A240785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of 4×n binary arrays with 
 * no element equal to fewer vertical neighbors than horizontal neighbors,
 * first element forced to 0 (new values 0..1 introduced in row‑major order).
 * Computed by a transfer‑matrix dynamic programming.
 */
#include <stdbool.h>

#define NONE 16                 /* sentinel for “no column” */

/* compute whether column `curr` satisfies the condition when its left neighbour
 * is `prev` and its right neighbour is `next`.  `prev`/`next` may be NONE. */
static bool ok_tbl[17][16][17];
static bool tbl_inited = false;

static inline bool compute_ok(int prev, int curr, int next)
{
    for (int r = 0; r < 4; ++r) {
        int cur_bit = (curr >> r) & 1;
        int V = 0;                       /* vertical equal neighbours */
        if (r > 0 && ((curr >> (r - 1)) & 1) == cur_bit) ++V;
        if (r < 3 && ((curr >> (r + 1)) & 1) == cur_bit) ++V;
        int H = 0;                       /* horizontal equal neighbours */
        if (prev != NONE && ((prev >> r) & 1) == cur_bit) ++H;
        if (next != NONE && ((next >> r) & 1) == cur_bit) ++H;
        if (V < H) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour for 0 undefined */

    /* initialise the OK table once */
    if (!tbl_inited) {
        for (int prev = 0; prev <= NONE; ++prev)
            for (int curr = 0; curr < 16; ++curr)
                for (int next = 0; next <= NONE; ++next)
                    ok_tbl[prev][curr][next] = compute_ok(prev, curr, next);
        tbl_inited = true;
    }

    /* DP arrays: dp_prev[prev][current] stores counts up to the current column */
    seq_t dp_prev[17][16] = {0};
    seq_t dp_next[17][16] = {0};

    /* initial column (column 1): first element (row 0) must be 0 */
    for (int curr = 0; curr < 16; ++curr) {
        if ((curr & 1) == 0)           /* top row (bit 0) is 0 */
            dp_prev[NONE][curr] = 1;
    }

    /* process columns 2 .. n */
    for (idx_t col = 2; col <= n; ++col) {
        for (int i = 0; i <= NONE; ++i)
            for (int j = 0; j < 16; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev <= NONE; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t cur_cnt = dp_prev[prev][curr];
                if (!cur_cnt) continue;
                for (int next = 0; next < 16; ++next) {
                    if (!ok_tbl[prev][curr][next]) continue;
                    dp_next[curr][next] += cur_cnt;
                }
            }
        }
        /* copy dp_next into dp_prev for the next iteration */
        for (int i = 0; i <= NONE; ++i)
            for (int j = 0; j < 16; ++j)
                dp_prev[i][j] = dp_next[i][j];
    }

    /* final column check: right neighbour is NONE */
    seq_t result = 0;
    for (int prev = 0; prev <= NONE; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            seq_t cnt = dp_prev[prev][curr];
            if (!cnt) continue;
            if (ok_tbl[prev][curr][NONE])
                result += cnt;
        }
    }

    return result;
}
