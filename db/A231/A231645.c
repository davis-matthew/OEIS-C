/*
 * A231645  Number of n X 2 0..2 arrays with no element less than a strict
 *          majority of its horizontal and vertical neighbors.
 * https://oeis.org/A231645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   check_edge, check_interior
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts $n\times 2$ arrays of symbols
 * 0,1,2 in which no element is less than a strict majority
 * of its horizontal and vertical neighbours.
 *
 * The code performs a linear‑time dynamic‑programming over the
 * rows.  At each step we add a new row and enforce the
 * neighbour conditions for the two rows that become adjacent.
 * The state kept is the pair of the last two rows.
 * The transition checks:
 *   * the interior row (previous row) against its two vertical
 *     neighbours (check_interior)
 *   * the new row against its upper neighbour
 *     (check_edge curr, prev)
 *   * for the first added row (when the upper neighbour is absent)
 *     we additionally check the reverse direction to ensure both
 *     rows satisfy their edge conditions.
 *
 * The recurrence used for efficiency is an O(n) DP,
 * not the 11‑term recurrence from the OEIS entry.
 */

#include <stdbool.h>
#include "../utils/check_edge.h"
#include "../utils/check_interior.h"

/* Encode a row (a,b) with 0≤a,b≤2 as an integer 0..8. */
static inline int encode_row(int a, int b) { return a * 3 + b; }

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;        /* a(1)=3 */

    /* DP tables: dp_old[prevprev][prev] and dp_new[prev][curr]. */
    /* Index 9 in the first dimension is used as a sentinel
     * for "no upper row" when the first row is being added. */
    seq_t dp_old[10][9], dp_new[10][9];
    const int NONE = 9;

    /* Initialise: before any rows are added, there is exactly one
     * configuration for each possible first row. */
    for (int up = 0; up < 10; ++up)
        for (int row = 0; row < 9; ++row)
            dp_old[up][row] = 0;
    for (int r = 0; r < 9; ++r) dp_old[NONE][r] = 1;

    int nn = (int)n;           /* safe: n <= 10000 for this problem */

    for (int step = 2; step <= nn; ++step) {
        /* reset dp_new */
        for (int up = 0; up < 10; ++up)
            for (int row = 0; row < 9; ++row)
                dp_new[up][row] = 0;

        for (int up = 0; up < 10; ++up) {
            for (int prev = 0; prev < 9; ++prev) {
                seq_t ways = dp_old[up][prev];
                if (ways == 0) continue;

                for (int curr = 0; curr < 9; ++curr) {
                    bool ok = false;
                    if (up == NONE) {
                        /* Adding the second row: check both directions. */
                        if (check_edge(prev, curr) && check_edge(curr, prev))
                            ok = true;
                    } else {
                        /* Adding a subsequent row: check interior of prev
                         * and edge rule for the new row. */
                        if (check_interior(up, prev, curr) &&
                            check_edge(curr, prev))
                            ok = true;
                    }
                    if (ok) dp_new[prev][curr] += ways;
                }
            }
        }
        /* move dp_new to dp_old for next iteration */
        for (int up = 0; up < 10; ++up)
            for (int row = 0; row < 9; ++row)
                dp_old[up][row] = dp_new[up][row];
    }

    /* Sum over all final states: all pairs (up, prev) are valid. */
    seq_t ans = 0;
    for (int up = 0; up < 9; ++up)     /* up==NONE cannot appear after step>=2 */
        for (int prev = 0; prev < 9; ++prev)
            ans += dp_old[up][prev];

    return ans;
}
