/*
 * A237939  Number of (n+1)X(2+1) 0..2 arrays with the maximum minus the
 *          minimum of every 2X2 subblock equal.
 * https://oeis.org/A237939
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

/* A237939: Number of (n+1)×3 arrays over {0,1,2} where every 2×2 subblock has
   the same max–min difference.  The implementation uses dynamic programming
   over the 27 possible row patterns.  For each of the three possible
   differences (0,1,2) a transition matrix is pre‑computed once and then
   used to count all admissible arrays.  The result is the sum over the
   three differences. */
#include <stdbool.h>
#include <stdint.h>

#define ROWS 27

/* Trans[delta][r1][r2] is true iff rows r1 and r2 form a 2×2 block pair
   whose max–min difference equals delta (for both column pairs). */
static bool trans[3][ROWS][ROWS];
static uint8_t row_digits[ROWS][3];

static void init_trans(void)
{
    /* pre‑compute the digits for each row pattern */
    for (int r = 0; r < ROWS; ++r) {
        int t = r;
        row_digits[r][2] = t % 3; t /= 3;
        row_digits[r][1] = t % 3; t /= 3;
        row_digits[r][0] = t % 3;
    }

    for (int r1 = 0; r1 < ROWS; ++r1) {
        int a0 = row_digits[r1][0], a1 = row_digits[r1][1], a2 = row_digits[r1][2];
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int b0 = row_digits[r2][0], b1 = row_digits[r2][1], b2 = row_digits[r2][2];
            /* compute min/max over the two 2×2 blocks */
            int min0 = a0, max0 = a0;
            if (a1 < min0) min0 = a1; if (b0 < min0) min0 = b0; if (b1 < min0) min0 = b1;
            if (a1 > max0) max0 = a1; if (b0 > max0) max0 = b0; if (b1 > max0) max0 = b1;
            int diff0 = max0 - min0;

            int min1 = a1, max1 = a1;
            if (a2 < min1) min1 = a2; if (b1 < min1) min1 = b1; if (b2 < min1) min1 = b2;
            if (a2 > max1) max1 = a2; if (b1 > max1) max1 = b1; if (b2 > max1) max1 = b2;
            int diff1 = max1 - min1;

            for (int delta = 0; delta < 3; ++delta) {
                trans[delta][r1][r2] = (diff0 == delta && diff1 == delta);
            }
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init_trans(); initialized = true; }

    seq_t total = (seq_t)0;

    for (int delta = 0; delta < 3; ++delta) {
        seq_t cur[ROWS];
        seq_t nxt[ROWS];

        /* start with any first row */
        for (int r = 0; r < ROWS; ++r) cur[r] = (seq_t)1;
        for (int r = 0; r < ROWS; ++r) nxt[r] = (seq_t)0;

        /* make n transitions (for n+1 rows) */
        for (idx_t step = 0; step < n; ++step) {
            for (int r = 0; r < ROWS; ++r) nxt[r] = (seq_t)0;
            for (int r1 = 0; r1 < ROWS; ++r1) {
                seq_t val = cur[r1];
                if (val == (seq_t)0) continue;
                for (int r2 = 0; r2 < ROWS; ++r2) {
                    if (trans[delta][r1][r2]) nxt[r2] = nxt[r2] + val;
                }
            }
            for (int r = 0; r < ROWS; ++r) cur[r] = nxt[r];
        }

        seq_t cnt = (seq_t)0;
        for (int r = 0; r < ROWS; ++r) cnt = cnt + cur[r];
        total = total + cnt;
    }

    return total;
}
