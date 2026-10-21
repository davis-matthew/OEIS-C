/*
 * A207545  Number of nX3 0..2 arrays avoiding the pattern z-2 z-1 z in any
 *          row, column, nw-to-se diagonal or ne-to-sw antidiagonal.
 * https://oeis.org/A207545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} avoiding the pattern
 *            0‑1‑2 in every row, column, and in the two possible 3‑cell
 *            diagonals (main and anti). Computed by a transfer‑matrix dynamic
 *            programming with 27² states
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset is 1, a(1)=26 */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)676;

    /* number of possible rows (3^3) */
    const int R = 27;
    static int digit[27][3];
    static bool row_ok[27];
    static bool initialized = false;

    if (!initialized) {
        for (int r = 0; r < R; ++r) {
            int x = r;
            digit[r][0] = x % 3; x /= 3;
            digit[r][1] = x % 3; x /= 3;
            digit[r][2] = x % 3;
            row_ok[r] = !(digit[r][0] == 0 && digit[r][1] == 1 && digit[r][2] == 2);
        }
        initialized = true;
    }

    /* DP arrays for counts of states (prev2, prev1) */
    seq_t dp_cur[27 * 27];
    seq_t dp_next[27 * 27];
    for (int i = 0; i < 27 * 27; ++i) dp_cur[i] = 0;

    /* initialise with first two rows */
    for (int r0 = 0; r0 < R; ++r0)
        if (row_ok[r0])
            for (int r1 = 0; r1 < R; ++r1)
                if (row_ok[r1])
                    dp_cur[r0 * R + r1] = 1;

    /* add remaining rows 2 .. n-1 (zero based) */
    for (idx_t step = 2; step < n; ++step) {
        /* clear next dp */
        for (int i = 0; i < 27 * 27; ++i) dp_next[i] = 0;
        for (int prev2 = 0; prev2 < R; ++prev2) {
            for (int prev1 = 0; prev1 < R; ++prev1) {
                seq_t ways = dp_cur[prev2 * R + prev1];
                if (ways == 0) continue;
                /* try every possible new row */
                for (int cur = 0; cur < R; ++cur) {
                    if (!row_ok[cur]) continue;
                    /* column check: no 0-1-2 in any column */
                    bool ok = true;
                    for (int j = 0; j < 3; ++j) {
                        if (digit[prev2][j] == 0 &&
                            digit[prev1][j] == 1 &&
                            digit[cur][j] == 2) {
                            ok = false; break;
                        }
                    }
                    if (!ok) continue;
                    /* main diagonal: (prev2[0],prev1[1],cur[2]) */
                    if (digit[prev2][0] == 0 &&
                        digit[prev1][1] == 1 &&
                        digit[cur][2] == 2) continue;
                    /* anti diagonal: (prev2[2],prev1[1],cur[0]) */
                    if (digit[prev2][2] == 0 &&
                        digit[prev1][1] == 1 &&
                        digit[cur][0] == 2) continue;
                    /* transition valid */
                    dp_next[prev1 * R + cur] += ways;
                }
            }
        }
        /* swap arrays */
        for (int i = 0; i < 27 * 27; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum over all final states for the n‑th row */
    seq_t total = 0;
    for (int i = 0; i < 27 * 27; ++i)
        total += dp_cur[i];

    return total;
}
