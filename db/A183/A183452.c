/*
 * A183452  Number of n X 5 binary arrays with every 1 having exactly three
 *          king-move neighbors equal to 1.
 * https://oeis.org/A183452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A183452 – Number of n×5 binary arrays
 * where each 1 has exactly three king‑move neighbors equal to 1.
 *
 * Algorithm: Dynamic programming over rows.
 *   State: (prevPrevRow, prevRow) – 10 bits (states = 2¹⁰ = 1024).
 *   Transition: choose curRow (5 bits).  The triple
 *     (prevPrevRow, prevRow, curRow) is valid iff every 1 in prevRow
 *     has exactly 3 neighbors among its 8 king moves (including bits
 *     from prevPrevRow and curRow).  This is pre‑computed.
 *   For the first row the state is (0,0); the transition checks row 0
 *     against (0,firstRow) – trivial.  After n iterations the last
 *     row (prevRow) must satisfy the condition with below row = 0.
 *
 * Complexity: O(n · 32 · 32 · 32) ≈ O(n · 2¹⁶), which runs in well
 *              under the time limit for n ≤ 10⁴.  All arithmetic uses
 *              `seq_t` so it works for all supported integer sizes,
 *              including big integers.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Pre‑computation of valid triples of rows. */
static bool __precomp[32][32][32];
static bool __precomp_done = false;

static void
__precomp_init(void)
{
    if (__precomp_done)
        return;

    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            for (int c = 0; c < 32; ++c) {
                bool ok = true;
                for (int col = 0; col < 5; ++col) {
                    if (!(b >> col & 1))
                        continue;                 /* cell is 0 */
                    int cnt = 0;
                    /* 8 king neighbours */
                    for (int dr = -1; dr <= 1; ++dr) {
                        int row_bits = (dr == -1) ? a :
                                       (dr == 0)  ? b : c;
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0)
                                continue;           /* skip self */
                            int ncol = col + dc;
                            if (ncol < 0 || ncol >= 5)
                                continue;
                            if (row_bits >> ncol & 1)
                                ++cnt;
                        }
                    }
                    if (cnt != 3) {
                        ok = false;
                        break;
                    }
                }
                __precomp[a][b][c] = ok;
            }
        }
    }
    __precomp_done = true;
}

static const int STATES = 1024;  /* 2 rows of 5 bits each */

/* Main sequence function. */
seq_t
a(idx_t n)
{
    __precomp_init();

    seq_t dp[STATES];
    seq_t next[STATES];
    /* initialise dp to zeros */
    for (int i = 0; i < STATES; ++i)
        dp[i] = 0;
    dp[0] = 1; /* state (0,0) before any rows */

    /* Process each row. */
    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < STATES; ++i)
            next[i] = 0;
        for (int s = 0; s < STATES; ++s) {
            seq_t ways = dp[s];
            if (!ways)
                continue;
            int p0 = s >> 5;        /* prevPrev row */
            int p1 = s & 31;        /* prev row     */
            for (int cur = 0; cur < 32; ++cur) {
                if (!__precomp[p0][p1][cur])
                    continue;
                int ns = (p1 << 5) | cur;   /* new state */
                next[ns] += ways;
            }
        }
        /* copy next to dp without using memcpy (avoid double free) */
        for (int i = 0; i < STATES; ++i)
            dp[i] = next[i];
    }

    /* Final check: below row is all zeros. */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s) {
        seq_t ways = dp[s];
        if (!ways)
            continue;
        int p0 = s >> 5;        /* row n-2 */
        int p1 = s & 31;        /* row n-1 */
        if (!__precomp[p0][p1][0])
            continue;            /* below row is 0 */
        result += ways;
    }
    return result;
}
