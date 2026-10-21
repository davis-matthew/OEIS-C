/*
 * A236722  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×3 arrays over {0,1,2} such that
 * for every 2×2 subblock, max + upper median + lower median – min
 * is the same constant.
 *
 * The algorithm uses dynamic programming on the 27 possible rows.
 * For two consecutive rows r1=(a1,a2,a3) and r2=(b1,b2,b3) the two
 * 2×2 blocks are
 *   (a1,a2,b1,b2)  and  (a2,a3,b2,b3).
 * Let val be the common value of the two blocks:
 *   val = sum - 2*min, where sum is the sum of the four entries.
 * The value val can be 0,1,2,3,4,5 or 6.
 *
 * Two rows are compatible if they produce the same val.  While
 * adding a new row we must keep the common val unchanged, thus the DP
 * state is (row, val).  The transition from row r to row r' is
 * possible iff pair_val[r][r']>=0 and pair_val[r][r'] == val.
 *
 * The DP runs in O(n·ROWS²) time and works for any seq_t.
 */

#include <stdbool.h>

#define ROWS 27      /* 3³ possible rows */
#define VCOUNT 7     /* values 0…6 */

static bool tables_initialized = false;
static signed char row_vals[ROWS][3];
static signed char pair_val[ROWS][ROWS];

/* Pre‑compute row encodings and pair values */
static void init_tables(void)
{
    for (idx_t r = 0; r < ROWS; ++r) {
        row_vals[r][0] = (signed char)(r / 9);
        row_vals[r][1] = (signed char)((r / 3) % 3);
        row_vals[r][2] = (signed char)(r % 3);
    }

    for (idx_t i = 0; i < ROWS; ++i) {
        for (idx_t j = 0; j < ROWS; ++j) {
            int a1 = row_vals[i][0];
            int a2 = row_vals[i][1];
            int a3 = row_vals[i][2];
            int b1 = row_vals[j][0];
            int b2 = row_vals[j][1];
            int b3 = row_vals[j][2];

            /* first 2×2 block (a1,a2,b1,b2) */
            int sum1 = a1 + a2 + b1 + b2;
            int min1 = a1;
            if (a2 < min1) min1 = a2;
            if (b1 < min1) min1 = b1;
            if (b2 < min1) min1 = b2;
            int val1 = sum1 - 2 * min1;

            /* second 2×2 block (a2,a3,b2,b3) */
            int sum2 = a2 + a3 + b2 + b3;
            int min2 = a2;
            if (a3 < min2) min2 = a3;
            if (b2 < min2) min2 = b2;
            if (b3 < min2) min2 = b3;
            int val2 = sum2 - 2 * min2;

            if (val1 == val2)
                pair_val[i][j] = (signed char)val1;   /* 0‑6 */
            else
                pair_val[i][j] = -1;
        }
    }

    tables_initialized = true;
}

seq_t a(idx_t n)
{
    if (!tables_initialized)
        init_tables();

    /* dp[row][v] = number of arrays ending with ‘row’ having common
       block value v (0…6).  Use a small temporary array for the next
       step. */
    seq_t dp[ROWS][VCOUNT];
    seq_t dp_next[ROWS][VCOUNT];

    /* initialize counts for arrays of height 2 (n=1) */
    for (idx_t r = 0; r < ROWS; ++r)
        for (idx_t v = 0; v < VCOUNT; ++v)
            dp[r][v] = 0;

    for (idx_t r1 = 0; r1 < ROWS; ++r1) {
        for (idx_t r2 = 0; r2 < ROWS; ++r2) {
            signed char val = pair_val[r1][r2];
            if (val >= 0) {
                idx_t v = (idx_t)val;          /* value 0…6 */
                dp[r2][v] += (seq_t)1;
            }
        }
    }

    if (n == 1) {
        /* sum all states: total arrays of height 2 */
        seq_t total = 0;
        for (idx_t r = 0; r < ROWS; ++r)
            for (idx_t v = 0; v < VCOUNT; ++v)
                total += dp[r][v];
        return total;
    }

    /* add rows 3 … (n+1) */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear dp_next */
        for (idx_t r = 0; r < ROWS; ++r)
            for (idx_t v = 0; v < VCOUNT; ++v)
                dp_next[r][v] = 0;

        for (idx_t r = 0; r < ROWS; ++r) {
            for (idx_t v = 0; v < VCOUNT; ++v) {
                seq_t cur = dp[r][v];
                if (!cur)
                    continue;
                for (idx_t r2 = 0; r2 < ROWS; ++r2) {
                    signed char val = pair_val[r][r2];
                    if (val < 0)
                        continue;
                    idx_t v2 = (idx_t)val;
                    if (v2 != v)
                        continue;
                    dp_next[r2][v] += cur;
                }
            }
        }

        /* copy dp_next back into dp */
        for (idx_t r = 0; r < ROWS; ++r)
            for (idx_t v = 0; v < VCOUNT; ++v)
                dp[r][v] = dp_next[r][v];
    }

    /* sum over all final states */
    seq_t total = 0;
    for (idx_t r = 0; r < ROWS; ++r)
        for (idx_t v = 0; v < VCOUNT; ++v)
            total += dp[r][v];
    return total;
}
