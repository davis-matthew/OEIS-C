/*
 * A207747  Number of n X 3 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207747
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

/* A207747 – number of n×3 0/1 arrays avoiding 001,011 horizontally
 * and 001,100 vertically.
 *
 * The algorithm uses 3‑state DP: the last two rows of the array must
 * be drawn from the six admissible 3‑bit patterns that contain no
 * forbidden horizontal triple.  A transition from the previous two
 * rows to a new row is allowed iff for every column the vertical
 * triple does not match 001 or 100.  The DP table tracks the
 * counts of arrays ending with a given pair of rows.
 *
 * For n=1 the answer is simply the number of admissible rows (6).
 * For n≥2 we iterate rows 2…n updating the DP table.
 *
 * Complexity: O(n) time, O(1) memory.  Works for any seq_t type
 * implemented by the build (uintX_t, arbitrary‑precision, etc.).
 */

#include <stdbool.h>

/* The six 3‑bit rows that contain no forbidden horizontal triple. */
static const int ALLOWED_ROWS[6] = {0, 2, 4, 5, 6, 7};
/* Index representing “no previous row” (for the second‐last row). */
static const int SENTINEL = 6;

/* Test whether a vertical triple (p2,p1,new) is NOT forbidden
 * in any of the three columns.  Forbidden patterns are 001 and 100. */
static inline bool vertical_ok(int p2, int p1, int newRow)
{
    for (int col = 0; col < 3; ++col) {
        int b = (p2 >> col) & 1;
        int a = (p1 >> col) & 1;
        int n = (newRow >> col) & 1;
        if ((b == 0 && a == 0 && n == 1) || (b == 1 && a == 0 && n == 0))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* dp[prev1][prev2] – number of arrays ending with
     * prev1 as the last row and prev2 as the second‑last row
     * (prev2 = SENTINEL when there is no second‑last row). */
    seq_t dp[6][7]      = {{0}};
    seq_t dp_next[6][7] = {{0}};

    /* First row: any admissible row can be placed with no second‑last row. */
    for (int i = 0; i < 6; ++i)
        dp[i][SENTINEL] = 1;

    /* Process rows 2 … n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Clear dp_next */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j <= 6; ++j)
                dp_next[i][j] = 0;

        for (int i = 0; i < 6; ++i)          /* prev1 */
            for (int j = 0; j <= 6; ++j)      /* prev2 */
            {
                seq_t cnt = dp[i][j];
                if (cnt == 0) continue;

                for (int k = 0; k < 6; ++k)  /* new row */
                {
                    if (j == SENTINEL || vertical_ok(ALLOWED_ROWS[j],
                                                     ALLOWED_ROWS[i],
                                                     ALLOWED_ROWS[k]))
                    {
                        dp_next[k][i] = dp_next[k][i] + cnt;
                    }
                }
            }

        /* Copy dp_next back to dp */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j <= 6; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum all states to obtain the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j <= 6; ++j)
            total = total + dp[i][j];

    return total;
}
