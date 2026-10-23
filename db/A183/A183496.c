/*
 * A183496  Half the number of nX3 0..2 arrays with no element equal to the
 *          sum mod 3 of its horizontal and vertical neighbors.
 * https://oeis.org/A183496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183496: a(n) = ½ · (number of n×3 arrays over {0,1,2}
   with no cell equal to the sum (mod 3) of its horizontal and vertical
   neighbors).  The function computes the count of arrays by a
   deterministic dynamic program and then divides by two. */

#include <stdbool.h>

/* pre‑computed data for the 27 possible rows (base‑3 encoding) */
static int rows[27][3];                     /* each row as [left,center,right] */
static bool valid_no_up_down[27];           /* row alone, no neighbors */
static bool valid_top[27][27];              /* top row with below row */
static bool valid_mid[27][27][27];          /* middle row between above & below */
static bool valid_bottom[27][27];           /* bottom row with above row */
static bool precomputed = false;

static void init_tables(void)
{
    if (precomputed) return;

    /* decode rows */
    for (int r = 0; r < 27; ++r) {
        int v = r;
        rows[r][2] = v % 3; v /= 3;
        rows[r][1] = v % 3; v /= 3;
        rows[r][0] = v % 3;
    }

    /* single row validity (no horizontal or vertical neighbors) */
    for (int r = 0; r < 27; ++r) {
        bool ok = true;
        for (int i = 0; i < 3 && ok; ++i) {
            int sum = 0;
            if (i > 0) sum += rows[r][i-1];
            if (i < 2) sum += rows[r][i+1];
            if ((sum % 3) == rows[r][i]) ok = false;
        }
        valid_no_up_down[r] = ok;
    }

    /* top and bottom adjacency checks */
    for (int r0 = 0; r0 < 27; ++r0) {
        for (int r1 = 0; r1 < 27; ++r1) {
            /* top row (row r0) with maybe below row r1 */
            bool ok = true;
            for (int i = 0; i < 3 && ok; ++i) {
                int sum = rows[r1][i];
                if (i > 0) sum += rows[r0][i-1];
                if (i < 2) sum += rows[r0][i+1];
                if ((sum % 3) == rows[r0][i]) ok = false;
            }
            valid_top[r0][r1] = ok;

            /* bottom row (row r1) with above row r0 */
            ok = true;
            for (int i = 0; i < 3 && ok; ++i) {
                int sum = rows[r0][i];
                if (i > 0) sum += rows[r1][i-1];
                if (i < 2) sum += rows[r1][i+1];
                if ((sum % 3) == rows[r1][i]) ok = false;
            }
            valid_bottom[r0][r1] = ok;
        }
    }

    /* middle row validity between two neighbours */
    for (int r0 = 0; r0 < 27; ++r0) {
        for (int r1 = 0; r1 < 27; ++r1) {
            for (int r2 = 0; r2 < 27; ++r2) {
                bool ok = true;
                for (int i = 0; i < 3 && ok; ++i) {
                    int sum = rows[r0][i] + rows[r2][i];
                    if (i > 0) sum += rows[r1][i-1];
                    if (i < 2) sum += rows[r1][i+1];
                    if ((sum % 3) == rows[r1][i]) ok = false;
                }
                valid_mid[r0][r1][r2] = ok;
            }
        }
    }

    precomputed = true;
}

/* DP implementation for the full array count */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;            /* undefined offset, defensive */
    if (n == 1) {
        seq_t count = 0;
        for (int r = 0; r < 27; ++r)
            if (valid_no_up_down[r]) count += 1;
        return count / 2; /* halve the count */
    }

    /* DP over rows: dp[x][y] = number of ways to end with rows x (above)
       and y (current) after processing some prefix */
    static seq_t dp[27][27], ndp[27][27];

    /* clear dp */
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dp[i][j] = 0;

    /* initial pair: first two rows (top with below) */
    for (int r0 = 0; r0 < 27; ++r0)
        for (int r1 = 0; r1 < 27; ++r1)
            if (valid_top[r0][r1])
                dp[r0][r1] = 1;

    /* iterate over remaining rows */
    for (idx_t row = 2; row < n; ++row) { /* row indices: 0-based, 0..n-1 */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                ndp[i][j] = 0;

        for (int r0 = 0; r0 < 27; ++r0)
            for (int r1 = 0; r1 < 27; ++r1) {
                seq_t ways = dp[r0][r1];
                if (!ways) continue;
                for (int r2 = 0; r2 < 27; ++r2)
                    if (valid_mid[r0][r1][r2])
                        ndp[r1][r2] += ways;
            }

        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row must satisfy bottom condition */
    seq_t total = 0;
    for (int r0 = 0; r0 < 27; ++r0)
        for (int r1 = 0; r1 < 27; ++r1) {
            seq_t ways = dp[r0][r1];
            if (!ways) continue;
            if (valid_bottom[r0][r1]) total += ways;
        }

    return total / 2;   /* halve the final count */
}
