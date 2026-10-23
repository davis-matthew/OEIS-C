/*
 * A198621  Number of nX2 0..5 arrays with values 0..5 introduced in row major
 *          order and each element equal to at least one horizontal or
 *          vertical neighbor.
 * https://oeis.org/A198621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198621: number of n×2 0…5 arrays introduced in row‑major order
   such that every element is equal to at least one horizontal or vertical
   neighbor.  Computed by dynamic programming over rows.  */

#include <stddef.h>
#include <stdint.h>

#define COLORS 6          /* 0…5 */
#define FLAGS 4           /* f0<<1 | f1 */
#define MAX_M 6           /* number of distinct colors used (1…6) */
#define STATE_COUNT (COLORS*COLORS*FLAGS*MAX_M)   /* 864 states */

static inline int state_index(int p0, int p1, int f0, int f1, int m)
{
    /* m is 1…6, store as 0…5 */
    return ((p0*COLORS + p1)*FLAGS + (f0*2 + f1))*MAX_M + (m-1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour below is irrelevant */

    seq_t dp[STATE_COUNT];
    seq_t nxt[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;

    /* --------- first row (row 0) --------- */
    /* left cell must be 0.  Right cell may be 0 or 1 (new color). */
    /* state (p0,p1,f0,f1,m) after the first row: */
    /*   0 0   -> m = 1, flags 0 0  (valid)
        0 1   -> m = 2, flags 1 1  (pending)
    */
    dp[state_index(0,0,0,0,1)] = 1;                /* 0 0   */
    dp[state_index(0,1,1,1,2)] = 1;                /* 0 1   */

    if (n == 1) {   /* only the first row remains */
        seq_t ans = 0;
        for (int p0 = 0; p0 < COLORS; ++p0)
            for (int p1 = 0; p1 < COLORS; ++p1)
                for (int m = 1; m <= MAX_M; ++m)
                    ans += dp[state_index(p0,p1,0,0,m)];
        return ans;
    }

    /* --------- subsequent rows (row >= 1) --------- */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = 0;

        for (int p0 = 0; p0 < COLORS; ++p0) {
            for (int p1 = 0; p1 < COLORS; ++p1) {
                for (int f0p = 0; f0p <= 1; ++f0p) {
                    for (int f1p = 0; f1p <= 1; ++f1p) {
                        for (int m = 1; m <= MAX_M; ++m) {
                            int idx = state_index(p0,p1,f0p,f1p,m);
                            seq_t val = dp[idx];
                            if (!val) continue;

                            /* possible colors for left cell */
                            for (int c0 = 0; c0 <= m && c0 < COLORS; ++c0) {
                                /* current max after left cell */
                                int m1 = m + (c0 == m && m < MAX_M ? 1 : 0);

                                /* must satisfy unsatisfied previous left cell if any */
                                if (f0p && c0 != p0) continue;

                                /* possible colors for right cell */
                                for (int c1 = 0; c1 <= m1 && c1 < COLORS; ++c1) {
                                    int m2 = m1 + (c1 == m1 && m1 < MAX_M ? 1 : 0);

                                    /* must satisfy unsatisfied previous right cell if any */
                                    if (f1p && c1 != p1) continue;

                                    /* flags for current row cells */
                                    int left_ok  = (c0 == c1) || (c0 == p0);
                                    int right_ok = (c1 == c0) || (c1 == p1);
                                    int f0c = left_ok  ? 0 : 1;
                                    int f1c = right_ok ? 0 : 1;

                                    int nn = state_index(c0,c1,f0c,f1c,m2);
                                    nxt[nn] += val;
                                }
                            }
                        }
                    }
                }
            }
        }

        /* move to next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = nxt[i];
    }

    /* final sum: only states where both flags are 0 are valid */
    seq_t ans = 0;
    for (int p0 = 0; p0 < COLORS; ++p0)
        for (int p1 = 0; p1 < COLORS; ++p1)
            for (int m = 1; m <= MAX_M; ++m)
                ans += dp[state_index(p0,p1,0,0,m)];

    return ans;
}
