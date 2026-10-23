/*
 * A187933  Number of (n+2)X4 binary arrays with every 2X2 subblock sum equal
 *          to some diagonal or antidiagonal neighbor 2X2 subblock sum.
 * https://oeis.org/A187933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A187933
    Number of (n+2)X4 binary arrays where every 2X2 subblock sum is
    equal to at least one of its diagonal or antidiagonal neighbour
    2X2 subblock sums.
    Computation uses dynamic programming over three consecutive rows.
*/
#include <stdint.h>
#include <string.h>

static uint8_t sum_ab[16][16][3];
static uint8_t satlower[16][16][16];
static uint8_t newmask[16][16][16];
static int init_done = 0;

static void init_tables(void)
{
    if (init_done) return;
    init_done = 1;

    /* pre‑compute 2x2 block sums */
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int j = 0; j < 3; ++j) {
                uint8_t bits = ((a >> j) & 1) + ((a >> (j+1)) & 1)
                            + ((b >> j) & 1) + ((b >> (j+1)) & 1);
                sum_ab[a][b][j] = bits;
            }
        }
    }

    /* which blocks of a are satisfied by lower neighbor blocks (b,c) */
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int c = 0; c < 16; ++c) {
                uint8_t sat = 0;
                for (int j = 0; j < 3; ++j) {
                    int ok = 0;
                    if (j > 0 && sum_ab[a][b][j] == sum_ab[b][c][j-1]) ok = 1;
                    if (!ok && j < 2 && sum_ab[a][b][j] == sum_ab[b][c][j+1]) ok = 1;
                    if (ok) sat |= 1U << j;
                }
                satlower[a][b][c] = sat;

                /* unsatisfied blocks of row b after seeing rows (a,b,c);
                   a block needs to match some diagonal neighbour,
                   which at this point can only be the block in (a,b) */
                uint8_t unsat = 0;
                for (int j = 0; j < 3; ++j) {
                    int ok = 0;
                    if (j > 0 && sum_ab[b][c][j] == sum_ab[a][b][j-1]) ok = 1;
                    if (!ok && j < 2 && sum_ab[b][c][j] == sum_ab[a][b][j+1]) ok = 1;
                    if (!ok) unsat |= 1U << j;
                }
                newmask[a][b][c] = unsat;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* below offset */
    init_tables();

    /* DP arrays: dp[a][b][pm] where pm are pending bits of row a */
    static seq_t dp[16][16][8];
    static seq_t ndp[16][16][8];
    memset(dp, 0, sizeof(dp));

    /* initialise all first two rows with all pending bits set (111) */
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            dp[r0][r1][7] = 1;

    /* we will have total (n+2) rows; already have first two,
       add rows 2 .. n+1 (n steps) */
    for (idx_t step = 2; step <= n + 1; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (int a = 0; a < 16; ++a) {
            for (int b = 0; b < 16; ++b) {
                for (int pm = 0; pm < 8; ++pm) {
                    seq_t cur = dp[a][b][pm];
                    if (!cur) continue;
                    for (int c = 0; c < 16; ++c) {
                        /* all pending blocks of row a must be satisfied
                           by the two lower rows b and c */
                        if ((pm & (~satlower[a][b][c])) != 0) continue;

                        /* compute unsatisfied bits of row b for future */
                        uint8_t nm = newmask[a][b][c];
                        ndp[b][c][nm] += cur;
                    }
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* sum over final states where the last processed row has no pending bits */
    seq_t ans = 0;
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            ans += dp[a][b][0];

    return ans;
}
