/*
 * A205239  Number of (n+1)X2 0..2 arrays with the number of rightwards and
 *          downwards edge increases in each 2X2 subblock differing from the
 *          number in each of its horizontal and vertical neighbors.
 * https://oeis.org/A205239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic‑programming over rows. The 2×(n+1) array is processed
 *            row by row. Let f(r1,r2) be the number of rightward or downward
 *            increases in the 2×2 block formed by adjacent rows r1 and r2.
 *            Then the array is counted by a linear recurrence on 9×5 states,
 *            where the state stores the current row (9 pos...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function returning a(a(n)) for OEIS A205239 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1; return 0 as safe value */

    const int ROWS  = 9;     /* 3×3 possible rows (0..2 in each column) */
    const int MAXC  = 5;     /* counts 0..4 */

    seq_t curr[ROWS][MAXC];
    seq_t next[ROWS][MAXC];

    /* zero arrays */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < MAXC; ++j)
            curr[i][j] = 0;

    /* initialise with first two rows, producing the first 2×2 block */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        int a0 = r0 / 3, b0 = r0 % 3;
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int a1 = r1 / 3, b1 = r1 % 3;
            int c = (a0 < b0) + (a1 < b1) + (a0 < a1) + (b0 < b1);
            curr[r1][c] = curr[r1][c] + (seq_t)1;
        }
    }

    /* process the remaining rows (total rows = n+1, we already have 2) */
    for (idx_t step = 3; step <= n + 1; ++step) {
        /* zero next state */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < MAXC; ++j)
                next[i][j] = 0;

        for (int prev = 0; prev < ROWS; ++prev) {
            int a0 = prev / 3, b0 = prev % 3;
            for (int pc = 0; pc < MAXC; ++pc) {
                seq_t ways = curr[prev][pc];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < ROWS; ++nxt) {
                    int a1 = nxt / 3, b1 = nxt % 3;
                    int c = (a0 < b0) + (a1 < b1) + (a0 < a1) + (b0 < b1);
                    if (c != pc) {
                        next[nxt][c] = next[nxt][c] + ways; /* DP transition */
                    }
                }
            }
        }

        /* copy next to curr for next iteration */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < MAXC; ++j)
                curr[i][j] = next[i][j];
    }

    /* sum all final states to obtain the count of arrays */
    seq_t total = 0;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < MAXC; ++c)
            total = total + curr[r][c];

    return total;
}
