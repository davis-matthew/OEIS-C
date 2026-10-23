/*
 * A183365  Number of nX3 binary arrays with no element equal to the sum mod 3
 *          of its horizontal and vertical neighbors.
 * https://oeis.org/A183365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of binary n×3 arrays in which no element equals the sum mod 3 of
 * its horizontal and vertical neighbors.  The function uses a dynamic
 * programming model over the rows:
 *
 *   dp[i][prev][curr] = number of ways to construct the first i rows
 *   so that row i−1 has pattern 'prev' and row i has pattern 'curr'.
 *
 *  Row validity is checked only when the row is fully determined:
 *  - For the first row we wait until the second one arrives.
 *  - For intermediate rows we check each row when the preceding and
 *    following row are known.
 *  - After the last row has been appended we check it using only its
 *    upper neighbour.
 *
 * This requires at most 8×8 DP tables and runs in O(n) time.
 */
#include <stdint.h>
#include <stdbool.h>

static bool row_valid(int row, int up, int down)
{
    /* up or down == -1 indicates that neighbour is missing (edge of the array). */
    for (int c = 0; c < 3; ++c) {
        int h = 0;
        if (c > 0) h += (row >> (c - 1)) & 1;
        if (c < 2) h += (row >> (c + 1)) & 1;

        int v = 0;
        if (up != -1)  v += (up >> c) & 1;
        if (down != -1) v += (down >> c) & 1;

        if (((h + v) % 3) == ((row >> c) & 1))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence */
    if (n == 1) {
        /* Count valid single rows only with horizontal neighbours. */
        seq_t count = 0;
        for (int r = 0; r < 8; ++r) {
            if (row_valid(r, -1, -1))
                ++count;
        }
        return count;              /* should be 2 */
    }

    static seq_t dp1[8][8];
    static seq_t dp2[8][8];
    seq_t (*prev)[8] = dp1;
    seq_t (*curr)[8] = dp2;

    /* initialise i = 2 */
    for (int r1 = 0; r1 < 8; ++r1)
        for (int r2 = 0; r2 < 8; ++r2)
            prev[r1][r2] = row_valid(r1, -1, r2) ? 1 : 0;

    /* build rows 3 … n */
    for (idx_t i = 3; i <= n; ++i) {
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
                curr[a][b] = 0;

        for (int r1 = 0; r1 < 8; ++r1)        /* previous row */
            for (int r2 = 0; r2 < 8; ++r2) {  /* current row */
                seq_t ways = prev[r1][r2];
                if (!ways) continue;
                for (int r3 = 0; r3 < 8; ++r3) { /* new row */
                    if (row_valid(r2, r1, r3))
                        curr[r2][r3] += ways;
                }
            }
        /* swap dp tables */
        seq_t (*temp)[8] = prev;
        prev = curr;
        curr = temp;
    }

    /* final check for the last row */
    seq_t result = 0;
    for (int r1 = 0; r1 < 8; ++r1)        /* row n-1 */
        for (int r2 = 0; r2 < 8; ++r2) {  /* row n   */
            if (row_valid(r2, r1, -1))
                result += prev[r1][r2];
        }

    return result;
}
