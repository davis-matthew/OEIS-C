/*
 * A183513  Number of nX5 binary arrays with each element equal to either the
 *          sum mod 2 of its horizontal and vertical neighbors or the sum mod
 *          2 of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Count of n×5 binary arrays satisfying the following local constraint:
 * for every cell (i,j), its value must equal either
 *   (sum of horizontal and vertical neighbours) mod 2
 * or
 *   (sum of diagonal and antidiagonal neighbours) mod 2.
 *
 * The computation is performed by a transfer‑matrix DP over rows.
 * Each row is represented by a 5‑bit mask.  The DP keeps the two
 * most recent row masks and, when a new row is added, checks the
 * constraint for the middle row.  After the last row is processed
 * the final constraint (using a zero “outside” row) is checked.
 *
 * The algorithm has O(n * 32³) precomputation and O(n * 32² * avgTrans)
 * runtime with a very small constant, easily within the time limits.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Pre‑compute the validity table sat[up][mid][down] and transition list.
     * This is done only once, on the first call.                                    */
    /* ------------------------------------------------------------------ */
    static bool sat[32][32][32];
    static uint8_t nextCount[32][32];
    static uint8_t nextRows[32][32][32];
    static bool initialized = false;

    if (!initialized) {
        for (int up = 0; up < 32; ++up) {
            for (int mid = 0; mid < 32; ++mid) {
                int cnt = 0;
                for (int down = 0; down < 32; ++down) {
                    /* Check all 5 cells in the middle row */
                    bool ok = true;
                    for (int j = 0; j < 5 && ok; ++j) {
                        int midBit = (mid >> j) & 1;

                        /* Horizontal + Vertical sum */
                        int hsum = 0;
                        if (j > 0)        hsum ^= (mid >> (j - 1)) & 1;
                        if (j < 4)        hsum ^= (mid >> (j + 1)) & 1;
                        hsum ^= (up  >> j) & 1;
                        hsum ^= (down>> j) & 1;

                        /* Diagonal + Antidiagonal sum */
                        int dsum = 0;
                        if (j > 0)        dsum ^= (up   >> (j - 1)) & 1;
                        if (j < 4)        dsum ^= (up   >> (j + 1)) & 1;
                        if (j > 0)        dsum ^= (down >> (j - 1)) & 1;
                        if (j < 4)        dsum ^= (down >> (j + 1)) & 1;

                        if (midBit != hsum && midBit != dsum)
                            ok = false;
                    }

                    sat[up][mid][down] = ok;
                    if (ok) {
                        nextRows[up][mid][cnt++] = (uint8_t)down;
                    }
                }
                nextCount[up][mid] = (uint8_t)cnt;
            }
        }
        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /* DP over rows.  dp[prev][curr] = number of ways to reach a prefix
     * ending with two rows (prev, curr).                                    */
    /* ------------------------------------------------------------------ */
    seq_t dp[32][32] = {0};
    seq_t newdp[32][32];

    /* Initial state: first row can be any 5‑bit mask, previous row is 0 */
    for (int r = 0; r < 32; ++r)
        dp[0][r] = 1;

    /* Process rows 2 .. n */
    for (idx_t row = 1; row < n; ++row) {   /* row counts rows processed so far */
        /* zero newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                uint8_t cnt = nextCount[prev][curr];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint8_t next = nextRows[prev][curr][k];
                    newdp[curr][next] += val;
                }
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Final row must satisfy constraint with a zero below */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (sat[prev][curr][0])
                ans += dp[prev][curr];
        }
    }

    return ans;
}
