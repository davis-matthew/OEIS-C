/*
 * A231541  Number of nX5 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A231541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231541: number of n×5 binary arrays where no entry is less than
   a strict majority of its horizontal, vertical and antidiagonal neighbors.
   Computed by dynamic programming over rows. */

#include <stdbool.h>

#define NONE 32   /* sentinel for "no row" */

static unsigned char valid_mid[33][33][33];
static bool valid_mid_inited = false;

static void compute_valid_mid(void)
{
    if (valid_mid_inited) return;
    for (int up = 0; up <= 32; up++) {
        for (int mid = 0; mid < 32; mid++) {
            for (int down = 0; down <= 32; down++) {
                bool ok = true;
                uint8_t midrow = (uint8_t)mid;
                uint8_t uprow  = (uint8_t)up;
                uint8_t downrow= (uint8_t)down;
                for (int j = 0; j < 5; j++) {
                    uint8_t midbit = (midrow >> j) & 1;
                    int ncnt = 0, ones = 0;

                    /* horizontal neighbors */
                    if (j > 0) { ncnt++; ones += (midrow >> (j-1)) & 1; }
                    if (j < 4) { ncnt++; ones += (midrow >> (j+1)) & 1; }

                    /* vertical neighbors */
                    if (up != NONE)   { ncnt++; ones += (uprow >> j) & 1; }
                    if (down != NONE) { ncnt++; ones += (downrow >> j) & 1; }

                    /* antidiagonal neighbors */
                    if (up != NONE && j > 0)   { ncnt++; ones += (uprow >> (j-1)) & 1; }
                    if (down != NONE && j < 4) { ncnt++; ones += (downrow >> (j+1)) & 1; }

                    if (ones * 2 > ncnt && midbit == 0) {
                        ok = false;
                        break;
                    }
                }
                valid_mid[up][mid][down] = ok;
            }
        }
    }
    valid_mid_inited = true;
}

seq_t a(idx_t n)
{
    compute_valid_mid();

    if (n == 0) return 0;

    /* Special case n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int row = 0; row < 32; row++) {
            if (valid_mid[NONE][row][NONE]) ans += 1;
        }
        return ans;
    }

    /* DP arrays */
    static seq_t dp[33][33];
    static seq_t newdp[33][33];

    /* initialize all to zero */
    for (int up = 0; up <= 32; up++)
        for (int mid = 0; mid <= 32; mid++)
            dp[up][mid] = 0;

    /* initialization: first two rows */
    for (int r0 = 0; r0 < 32; r0++) {
        for (int r1 = 0; r1 < 32; r1++) {
            if (valid_mid[NONE][r0][r1]) {
                dp[r0][r1] = 1;
            }
        }
    }

    /* process remaining rows 2..n-1 */
    for (idx_t i = 2; i < n; i++) {
        for (int up = 0; up <= 32; up++)
            for (int mid = 0; mid <= 32; mid++)
                newdp[up][mid] = 0;

        for (int up = 0; up <= 32; up++) {
            for (int mid = 0; mid <= 32; mid++) {
                seq_t cur = dp[up][mid];
                if (!cur) continue;
                for (int curr = 0; curr < 32; curr++) {
                    if (valid_mid[up][mid][curr]) {
                        newdp[mid][curr] += cur;
                    }
                }
            }
        }

        for (int up = 0; up <= 32; up++)
            for (int mid = 0; mid <= 32; mid++)
                dp[up][mid] = newdp[up][mid];
    }

    /* final validation of the last row */
    seq_t ans = 0;
    for (int up = 0; up < 32; up++) {
        for (int mid = 0; mid < 32; mid++) {
            seq_t cur = dp[up][mid];
            if (!cur) continue;
            if (valid_mid[up][mid][NONE]) {
                ans += cur;
            }
        }
    }
    return ans;
}
