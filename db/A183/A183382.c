/*
 * A183382  Half the number of n X 3 binary arrays with no element equal to a
 *          strict majority of its king-move neighbors.
 * https://oeis.org/A183382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence. This implementation uses a transfer‑matrix
 *            (DP over 3‑column states) to enumerate n×3 binary arrays that
 *            satisfy the “no strict majority of king‑move neighbors”
 *            property. The answer returned is the sequence value a(n), which
 *            is half of that count. All calculations are perfor...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Local helper: check whether a middle row receives a valid assignment
   when surrounded by a previous and next row.  The arguments
   `hasPrev` and `hasNext` allow handling of border rows.  */
static bool check_row(int cur, int prev, int next, bool hasPrev, bool hasNext)
{
    for (int j = 0; j < 3; ++j) {
        int v = (cur >> j) & 1;      /* value of the cell (row i, column j) */
        int same = 0;                /* neighbors equal to v */
        int m = 0;                   /* total number of existing neighbors */

        /* horizontal neighbors in the same row */
        if (j > 0) {
            int nb = (cur >> (j - 1)) & 1;
            if (nb == v) same++;
            m++;
        }
        if (j < 2) {
            int nb = (cur >> (j + 1)) & 1;
            if (nb == v) same++;
            m++;
        }

        /* vertical neighbors on the previous row, if any */
        if (hasPrev) {
            for (int k = j - 1; k <= j + 1; ++k) {
                if (k >= 0 && k < 3) {
                    int nb = (prev >> k) & 1;
                    if (nb == v) same++;
                    m++;
                }
            }
        }

        /* vertical neighbors on the next row, if any */
        if (hasNext) {
            for (int k = j - 1; k <= j + 1; ++k) {
                if (k >= 0 && k < 3) {
                    int nb = (next >> k) & 1;
                    if (nb == v) same++;
                    m++;
                }
            }
        }

        /* strict majority check */
        int threshold = m / 2;  /* floor division */
        if (same > threshold)
            return false;
    }
    return true;
}

/* Pre‑computed tables for fast transitions */
static bool OK_TOP[8][8];               /* row1, row2 */
static bool OK_BOTTOM[8][8];            /* prev, curr */
static bool OK_MIDDLE[8][8][8];         /* prev, curr, next */
static bool tables_init = false;

static void init_tables(void)
{
    for (int cur = 0; cur < 8; ++cur) {
        for (int nxt = 0; nxt < 8; ++nxt) {
            /* Top boundary: no previous row, next row present */
            OK_TOP[cur][nxt] = check_row(cur, 0, nxt, false, true);

            /* Bottom boundary: previous row present, no next row */
            OK_BOTTOM[cur][nxt] = check_row(nxt, cur, 0, true, false);
        }
    }

    for (int prv = 0; prv < 8; ++prv) {
        for (int cur = 0; cur < 8; ++cur) {
            for (int nxt = 0; nxt < 8; ++nxt) {
                /* Middle rows: both neighbors present */
                OK_MIDDLE[prv][cur][nxt] = check_row(cur, prv, nxt, true, true);
            }
        }
    }

    tables_init = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!tables_init)
        init_tables();

    if (n == 0)          /* offset is 1, a(0) is unused */
        return 0;

    /* Special case n == 1: only horizontal neighbors exist */
    if (n == 1) {
        seq_t cnt = 0;
        for (int r = 0; r < 8; ++r)
            if (check_row(r, 0, 0, false, false))
                cnt += 1;
        return cnt / 2;
    }

    /* DP tables: dp[prev][curr] holds counts for processed rows up to curr
       with row curr validated and row prev still needing validation later. */
    seq_t dp[8][8] = {0};
    seq_t newdp[8][8] = {0};

    /* Initialize with first two rows (validate top row) */
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int r2 = 0; r2 < 8; ++r2) {
            if (OK_TOP[r1][r2])
                dp[r1][r2] = 1;
        }
    }

    /* Process rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero newdp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                newdp[i][j] = 0;

        for (int prv = 0; prv < 8; ++prv) {
            for (int cur = 0; cur < 8; ++cur) {
                seq_t cnt = dp[prv][cur];
                if (cnt == 0)
                    continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (OK_MIDDLE[prv][cur][nxt])
                        newdp[cur][nxt] += cnt;
                }
            }
        }

        /* copy newdp into dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Final validation of the last row (no next row) */
    seq_t total = 0;
    for (int prv = 0; prv < 8; ++prv) {
        for (int cur = 0; cur < 8; ++cur) {
            if (OK_BOTTOM[prv][cur])
                total += dp[prv][cur];
        }
    }

    return total / 2;
}
