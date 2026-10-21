/*
 * A202982  Number of n X 2 0..2 arrays with every nonzero element less than
 *          or equal to some NW, E or S neighbor.
 * https://oeis.org/A202982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×2 arrays with elements 0..2 where each nonzero
 *            element is <= at least one of its NW, E, or S neighbors.
 *            Computed by dynamic programming over rows
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 9 possible rows (0..2 for each of two columns).
       Map row index r (0..8) to its column values:
          col0 = r/3,  col1 = r%3   */
    static const uint8_t val0[9] = {0,0,0,1,1,1,2,2,2};
    static const uint8_t val1[9] = {0,1,2,0,1,2,0,1,2};

    /* Current DP state: dp_cur[row][mask]  (mask is 2‑bit unsatisfied flag) */
    seq_t dp_cur[9][4] = {{0}};
    seq_t dp_next[9][4];

    /* ----- initial row (row 1), virtual previous row is (0,0) ---- */
    for (int cur = 0; cur < 9; ++cur) {
        int mask = 0;

        /* column 0 : satisfied by E neighbor if possible */
        if (val0[cur] != 0) {
            if (!(val1[cur] > 0 && val1[cur] >= val0[cur]))
                mask |= 1;               /* needs S later */
        }

        /* column 1 : no NW neighbor in first row → needs S if non‑zero */
        if (val1[cur] != 0)
            mask |= 2;

        dp_cur[cur][mask] = 1;
    }

    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < 9; ++r) total += dp_cur[r][0];
        return total;
    }

    /* ----- iterate remaining rows (row 2 .. n) ----- */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 9; ++i)
            for (int m = 0; m < 4; ++m)
                dp_next[i][m] = 0;

        for (int prev = 0; prev < 9; ++prev) {
            uint8_t prev0 = val0[prev];
            uint8_t prev1 = val1[prev];
            for (int mask_prev = 0; mask_prev < 4; ++mask_prev) {
                seq_t cnt = dp_cur[prev][mask_prev];
                if (!cnt) continue;

                for (int cur = 0; cur < 9; ++cur) {
                    uint8_t cur0 = val0[cur];
                    uint8_t cur1 = val1[cur];
                    int ok = 1;

                    /* check unsatisfied cells in previous row via S */
                    if (prev0 != 0 && (mask_prev & 1)) {
                        if (!(cur0 > 0 && cur0 >= prev0)) ok = 0;
                    }
                    if (ok && prev1 != 0 && (mask_prev & 2)) {
                        if (!(cur1 > 0 && cur1 >= prev1)) ok = 0;
                    }
                    if (!ok) continue;

                    /* compute mask for current row (needs S later) */
                    int mask_cur = 0;
                    /* column 0 : satisfies by E neighbor */
                    if (cur0 != 0) {
                        if (!(cur1 > 0 && cur1 >= cur0)) mask_cur |= 1;
                    }
                    /* column 1 : satisfies by NW neighbor (prev0) */
                    if (cur1 != 0) {
                        if (!(prev0 > 0 && prev0 >= cur1)) mask_cur |= 2;
                    }

                    dp_next[cur][mask_cur] += cnt;
                }
            }
        }

        /* swap dp_cur and dp_next for next iteration */
        for (int i = 0; i < 9; ++i)
            for (int m = 0; m < 4; ++m)
                dp_cur[i][m] = dp_next[i][m];
    }

    /* After processing all rows, only states with mask 0 are valid */
    seq_t result = 0;
    for (int r = 0; r < 9; ++r) result += dp_cur[r][0];
    return result;
}
