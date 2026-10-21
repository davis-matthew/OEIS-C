/*
 * A231767  Number of (3+1)X(n+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   column_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231767: number of 4×(n+1) 0–1 arrays where no cell has a strict
 * majority of its horizontal, diagonal and antidiagonal neighbors equal to one.
 * Computation is done by a column‑wise dynamic program that keeps the last two
 * columns.  For each intermediate column it is verified against its left and
 * right neighbours.  The sentinel value LEFT_NIL (=16) denotes absence of a
 * neighbour (first or last column).  Time complexity is O(n·256); memory
 * consumption is constant. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/column_ok.h"

seq_t a(idx_t n)
{
    /* 4 rows, so 16 possible columns (4 bits).  Index 16 means no adjacent column. */
    #define LEFT_NIL 16
    #define LEFT_COUNT 17   /* 0–15 real + sentinel */
    #define COL_BITS   16   /* 4 bits */

    static seq_t dp[LEFT_COUNT][COL_BITS];
    static seq_t nxt[LEFT_COUNT][COL_BITS];

    /* initialise dp with all possibilities for the first column. */
    for (size_t i = 0; i < LEFT_COUNT; ++i)
        for (size_t j = 0; j < COL_BITS; ++j)
            dp[i][j] = 0;
    for (uint8_t c0 = 0; c0 < COL_BITS; ++c0)
        dp[LEFT_NIL][c0] = 1;

    /* iterate through columns 1 .. n (total n other columns after the first). */
    for (idx_t col = 1; col <= n; ++col) {
        for (size_t i = 0; i < LEFT_COUNT; ++i)
            for (size_t j = 0; j < COL_BITS; ++j)
                nxt[i][j] = 0;

        for (uint8_t left = 0; left < LEFT_COUNT; ++left) {
            const bool left_exists = (left != LEFT_NIL);
            for (uint8_t mid = 0; mid < COL_BITS; ++mid) {
                seq_t cnt = dp[left][mid];
                if (!cnt) continue;
                for (uint8_t r = 0; r < COL_BITS; ++r) {
                    if (column_ok(left, left_exists, mid, r, true)) {
                        nxt[mid][r] += cnt;
                    }
                }
            }
        }
        /* swap dp and nxt for next iteration */
        for (size_t i = 0; i < LEFT_COUNT; ++i)
            for (size_t j = 0; j < COL_BITS; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* validate the last column against right sentinel (no right neighbor). */
    seq_t ans = 0;
    for (uint8_t left = 0; left < LEFT_COUNT; ++left) {
        const bool left_exists = (left != LEFT_NIL);
        for (uint8_t mid = 0; mid < COL_BITS; ++mid) {
            seq_t cnt = dp[left][mid];
            if (!cnt) continue;
            if (column_ok(left, left_exists, mid, 0, false))
                ans += cnt;
        }
    }
    return ans;
}
