/*
 * A210168  Number of (n+1) X 3 0..3 arrays containing all values 0..3 with
 *          every 2 X 2 subblock having two or four distinct values, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A210168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A210168: a(n) = number of (n+1)×3 arrays of 0..3, each 2×2 subblock has 2 or 4 distinct values,
   and new values 0..3 arise in row‑major order.
   This implementation counts all such arrays by dynamic programming over rows. */

#include <stdint.h>
#include <stdlib.h>

/* helper to set a 4×64 table to zero */
static void zero_dp(seq_t arr[4][64]) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 64; ++j)
            arr[i][j] = 0;
}

seq_t a(idx_t n) {
    /* maximum n that will be cached – buffers large enough for n≈10000 */
    #define MAX_CACHE 20001

    static int initialized = 0;
    static idx_t rowsCurrent;          /* number of rows processed in DP   */
    static seq_t dp[4][64];            /* counts after current row          */
    static seq_t nextdp[4][64];        /* temporary storage for transition */
    static uint8_t rowVals[64][3];     /* decoded 2‑bit values of a row    */
    static seq_t cache[MAX_CACHE];    /* cached a(n) for n < MAX_CACHE    */

    /* initialize once */
    if (!initialized) {
        zero_dp(dp);
        zero_dp(nextdp);
        rowsCurrent = 0;  /* no rows processed yet */

        /* pre­decode all 64 possible rows */
        for (int row = 0; row < 64; ++row) {
            for (int c = 0; c < 3; ++c)
                rowVals[row][c] = (row >> (2 * c)) & 3;
        }

        /* first row (row 0) – must start with 0 and obey order constraint */
        for (int row = 0; row < 64; ++row) {
            if (rowVals[row][0] != 0)      /* first entry must be 0 */
                continue;

            int curmax = 0;
            int ok = 1;
            for (int c = 0; c < 3; ++c) {
                int val = rowVals[row][c];
                if (val > curmax + 1) { ok = 0; break; }
                if (val == curmax + 1) ++curmax;
            }
            if (!ok) continue;
            dp[curmax][row] += 1;          /* count this first row configuration */
        }

        rowsCurrent = 1;                 /* processed one row */
        initialized = 1;
    }

    if (n == 0)
        return 0;  /* defined only for n ≥ 1 */

    /* extend DP until we have processed n+1 rows */
    while (rowsCurrent < n + 1) {
        zero_dp(nextdp);                 /* reset nextdp for this transition */

        /* transition from current row to next row */
        for (int used = 0; used < 4; ++used) {
            for (int prev = 0; prev < 64; ++prev) {
                seq_t curcnt = dp[used][prev];
                if (curcnt == 0) continue;

                const uint8_t *pv = rowVals[prev];

                for (int nxt = 0; nxt < 64; ++nxt) {
                    const uint8_t *nv = rowVals[nxt];

                    /* 2×2 subblocks must have 2 or 4 distinct values */
                    int lmask = (1 << pv[0]) | (1 << pv[1]) |
                                (1 << nv[0]) | (1 << nv[1]);
                    int rmask = (1 << pv[1]) | (1 << pv[2]) |
                                (1 << nv[1]) | (1 << nv[2]);

                    if ( __builtin_popcount(lmask) != 2 &&
                         __builtin_popcount(lmask) != 4 ) continue;
                    if ( __builtin_popcount(rmask) != 2 &&
                         __builtin_popcount(rmask) != 4 ) continue;

                    /* order constraint for the new row */
                    int cur = used;
                    int ok = 1;
                    for (int c = 0; c < 3; ++c) {
                        int val = nv[c];
                        if (val > cur + 1) { ok = 0; break; }
                        if (val == cur + 1) ++cur;
                    }
                    if (!ok) continue;

                    /* accumulate the count */
                    nextdp[cur][nxt] += curcnt;
                }
            }
        }

        /* move to next iteration */
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = nextdp[i][j];

        ++rowsCurrent;  /* another row processed */

        /* store a(rowsCurrent-1) if we haven't cached it yet */
        idx_t curN = rowsCurrent - 1;
        if (curN < MAX_CACHE) {
            seq_t total = 0;
            for (int r = 0; r < 64; ++r)
                total += dp[3][r];     /* only states with max value 3 count */
            cache[curN] = total;
        }
    }

    return cache[n];
}
