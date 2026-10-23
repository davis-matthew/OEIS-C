/*
 * A025599  Number of n-move knight paths on 8 X 8 board from given corner to
 *          any square.
 * https://oeis.org/A025599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n-move knight paths on an 8×8 board from a
 *            corner to any square
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP computation: iterate n steps, accumulating path counts for each square. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* adjacency lists for knight moves ---------------------------------------------------------- */
    static int neigh[64][8];
    static int neigh_cnt[64];
    static bool table_inited = false;
    if (!table_inited) {
        static const int dx[8] = { 2,  2, -2, -2,  1,  1, -1, -1 };
        static const int dy[8] = { 1, -1,  1, -1,  2, -2,  2, -2 };
        for (int x = 0; x < 8; ++x)
            for (int y = 0; y < 8; ++y) {
                int pos = x * 8 + y;
                int cnt = 0;
                for (int m = 0; m < 8; ++m) {
                    int nx = x + dx[m];
                    int ny = y + dy[m];
                    if (0 <= nx && nx < 8 && 0 <= ny && ny < 8)
                        neigh[pos][cnt++] = nx * 8 + ny;
                }
                neigh_cnt[pos] = cnt;
            }
        table_inited = true;
    }

    /* DP arrays: dp holds counts after current step, next for next step -------------------------------- */
    static seq_t dp[64];
    static seq_t next[64];

    /* step 0 : 1 path at the start corner (index 0) --------------------------------------------------- */
    if (n == 0)
        return (seq_t)1;

    memset(dp, 0, sizeof(dp));
    dp[0] = (seq_t)1;

    seq_t total = 0;  /* will hold a(n) after the loop */

    /* iterate from step 1 to n ------------------------------------------------------------- */
    for (idx_t step = 1; step <= n; ++step) {
        memset(next, 0, sizeof(next));

        for (int pos = 0; pos < 64; ++pos) {
            seq_t ways = dp[pos];
            if (ways == (seq_t)0) continue;
            for (int k = 0; k < neigh_cnt[pos]; ++k) {
                int to = neigh[pos][k];
                next[to] = next[to] + ways;
            }
        }

        /* after propagation, dp will contain counts for this step ----------------------------- */
        memcpy(dp, next, sizeof(dp));

        /* sum over all squares to get total number of paths of length 'step' -------------------- */
        total = (seq_t)0;
        for (int pos = 0; pos < 64; ++pos)
            total = total + dp[pos];
    }

    return total;
}
