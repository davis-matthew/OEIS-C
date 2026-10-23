/*
 * A186134  Number of (n+1)X3 0..2 arrays with no 2X2 subblock trace equal to
 *          any horizontal or vertical neighbor 2X2 subblock trace.
 * https://oeis.org/A186134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation by dynamic programming over rows.
 * We consider all 3‑digit rows over {0,1,2} (27 possibilities).
 * For each pair of consecutive rows we compute the two
 * 2×2 subblock traces: T0 = r[i][0] + r[i+1][1],
 *                           T1 = r[i][1] + r[i+1][2].
 * Constraints:  T0 != T1 (horizontal neighbour),
 *             and for consecutive pairs B(i−1,j) and B(i,j)
 *             the traces on the same column must differ
 *             (vertical neighbour).  The dynamic programming
 *             keeps track of the previous row and the two
 *             traces of the previous pair; it then iterates
 *             over the next row, checks all constraints,
 *             and updates the state.
 * Complexity: O(n · 27 · 25 · 27) ≈ O(18 000·n),
 * which easily meets the time limit for n up to 10 000.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* Handle undefined case; the offset is 1 but we return 0 for n==0. */
    if (n == 0) return 0;

    /* Precompute all possible 3‑digit rows in base 3. */
    static const int rows[27][3] = {
        {0,0,0},{0,0,1},{0,0,2},{0,1,0},{0,1,1},{0,1,2},
        {0,2,0},{0,2,1},{0,2,2},{1,0,0},{1,0,1},{1,0,2},
        {1,1,0},{1,1,1},{1,1,2},{1,2,0},{1,2,1},{1,2,2},
        {2,0,0},{2,0,1},{2,0,2},{2,1,0},{2,1,1},{2,1,2},
        {2,2,0},{2,2,1},{2,2,2}
    };

    /* DP tables: dp_cur[row][t0][t1], dp_next same size. */
    static seq_t dp_cur[27][5][5];
    static seq_t dp_next[27][5][5];

    /* Zero the tables. */
    for (int r = 0; r < 27; ++r)
        for (int t0 = 0; t0 < 5; ++t0)
            for (int t1 = 0; t1 < 5; ++t1) {
                dp_cur[r][t0][t1] = 0;
                dp_next[r][t0][t1] = 0;
            }

    /* Base case: arrays of length 2 (n==1). */
    for (int r0 = 0; r0 < 27; ++r0)
        for (int r1 = 0; r1 < 27; ++r1) {
            int t0 = rows[r0][0] + rows[r1][1];
            int t1 = rows[r0][1] + rows[r1][2];
            if (t0 != t1) {
                dp_cur[r1][t0][t1] += 1;
            }
        }

    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 27; ++r)
            for (int t0 = 0; t0 < 5; ++t0)
                for (int t1 = 0; t1 < 5; ++t1)
                    ans += dp_cur[r][t0][t1];
        return ans;
    }

    /* Iterate for rows 3 through n+1 (total n+1 rows). */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero dp_next. */
        for (int r = 0; r < 27; ++r)
            for (int t0 = 0; t0 < 5; ++t0)
                for (int t1 = 0; t1 < 5; ++t1)
                    dp_next[r][t0][t1] = 0;

        for (int r_pre = 0; r_pre < 27; ++r_pre)
            for (int t0_prev = 0; t0_prev < 5; ++t0_prev)
                for (int t1_prev = 0; t1_prev < 5; ++t1_prev) {
                    seq_t cnt = dp_cur[r_pre][t0_prev][t1_prev];
                    if (cnt == 0) continue;

                    for (int r_next = 0; r_next < 27; ++r_next) {
                        int t0_next = rows[r_pre][0] + rows[r_next][1];
                        int t1_next = rows[r_pre][1] + rows[r_next][2];
                        if (t0_next != t1_next &&
                            t0_next != t0_prev &&
                            t1_next != t1_prev) {
                            dp_next[r_next][t0_next][t1_next] += cnt;
                        }
                    }
                }

        /* Swap dp_cur and dp_next for next iteration. */
        for (int r = 0; r < 27; ++r)
            for (int t0 = 0; t0 < 5; ++t0)
                for (int t1 = 0; t1 < 5; ++t1)
                    dp_cur[r][t0][t1] = dp_next[r][t0][t1];
    }

    /* Sum all states to get the answer. */
    seq_t result = 0;
    for (int r = 0; r < 27; ++r)
        for (int t0 = 0; t0 < 5; ++t0)
            for (int t1 = 0; t1 < 5; ++t1)
                result += dp_cur[r][t0][t1];

    return result;
}
