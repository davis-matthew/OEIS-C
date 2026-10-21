/*
 * A237946  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays with entries in {0,1,2,3} such
 *            that for every 2×2 subblock the value (max + upper median +
 *            lower median) differs from the value of the subblock above/below
 *            it by exactly one. Computed by dynamic programming over rows
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

seq_t a(idx_t n)
{
    /* Pre‑compute value of a 2×2 block from two rows.
       encode a row as a 4‑bit number: row = high*4 + low
       where high, low are values 0..3 for columns 0 and 1. */
    static int val[16][16];
    static int precomp_done = 0;
    if (!precomp_done) {
        for (int r1 = 0; r1 < 16; ++r1) {
            int a = r1 / 4;               /* column 0 of row r1 */
            int b = r1 % 4;               /* column 1 of row r1 */
            for (int r2 = 0; r2 < 16; ++r2) {
                int c = r2 / 4;           /* column 0 of row r2 */
                int d = r2 % 4;           /* column 1 of row r2 */
                int s = a + b + c + d;    /* sum of the four cells */
                int m = a;
                if (b < m) m = b;
                if (c < m) m = c;
                if (d < m) m = d;
                val[r1][r2] = s - m;      /* sum of the three largest numbers */
            }
        }
        precomp_done = 1;
    }

    /* dp arrays: dp_prev[row][v] = number of ways to form rows up to the last
       row 'row', with the value of the last subblock being 'v' (0..9). */
    static seq_t dp_prev[16][10];
    static seq_t dp_cur[16][10];
    memset(dp_prev, 0, sizeof(dp_prev));

    /* Base case: pick first two rows arbitrarily. */
    for (idx_t r0 = 0; r0 < 16; ++r0) {
        for (idx_t r1 = 0; r1 < 16; ++r1) {
            seq_t cnt = 1;                   /* one choice for this pair */
            int v = val[r0][r1];
            dp_prev[r1][v] += cnt;
        }
    }

    /* For n==1 we already counted all 2×2 arrays. */
    if (n == 1ULL) {
        seq_t sum = 0;
        for (idx_t r = 0; r < 16; ++r)
            for (idx_t v = 0; v < 10; ++v)
                sum += dp_prev[r][v];
        return sum;
    }

    /* Iterate for rows 2..n. */
    for (idx_t step = 2; step < n + 1; ++step) {
        memset(dp_cur, 0, sizeof(dp_cur));
        for (idx_t prev_row = 0; prev_row < 16; ++prev_row) {
            for (idx_t vprev = 0; vprev < 10; ++vprev) {
                seq_t ways = dp_prev[prev_row][vprev];
                if (ways == 0) continue;
                for (idx_t new_row = 0; new_row < 16; ++new_row) {
                    int vcurr = val[prev_row][new_row];
                    int diff = (vcurr > vprev) ? vcurr - vprev : vprev - vcurr;
                    if (diff == 1) {
                        dp_cur[new_row][vcurr] += ways;
                    }
                }
            }
        }
        /* swap dp_prev and dp_cur */
        for (idx_t r = 0; r < 16; ++r)
            for (idx_t v = 0; v < 10; ++v)
                dp_prev[r][v] = dp_cur[r][v];
    }

    /* Sum all counts for the last row. */
    seq_t total = 0;
    for (idx_t r = 0; r < 16; ++r)
        for (idx_t v = 0; v < 10; ++v)
            total += dp_prev[r][v];
    return total;
}
