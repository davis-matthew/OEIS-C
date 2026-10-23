/*
 * A184609  Half the number of (n+1)X5 binary arrays with no 2X2 subblock sum
 *          differing from a horizontal or vertical neighbor subblock sum by
 *          more than one.
 * https://oeis.org/A184609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A184609
 *  a(n) = half the number of (n+1)×5 binary arrays whose every 2×2 subblock
 *  sum differs from each horizontal or vertical neighbour subblock sum by at
 *  most one.  The implementation follows the definition directly: row
 *  patterns of length 5 are represented as 5‑bit numbers (0 .. 31).  For each
 *  pair of consecutive rows we pre‑compute the four 2×2 block sums and the
 *  allowed transitions to a third row.  Dynamic programming over the rows
 *  yields the total number; the returned value is that total divided by 2.
 *
 *  The code uses only integer arithmetic (+, -, *, /, %), comparisons and
 *  bitwise operators.  It contains no floating‑point operations and is
 *  compatible with C11 and C++17.  It works for all supported
 *  seq_t/idx_t types without any changes.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ROWS 32                      /* 5-bit rows -> 0 .. 31 */

/* ------------------------------------------------------------------ */
/* Precomputation of transition tables, executed once on the first call */
static bool ready = false;           /* have we built the tables? */

/* 4‑bit sums for every ordered pair of rows */
static unsigned char pair_sum[ROWS][ROWS][4];      /* 32×32×4 */

/* Horizontal constraint: pair_sum differences across the 3 adjacent blocks */
static bool horiz_ok[ROWS][ROWS];                 /* 32×32 */

/* Vertical constraint: pair_sum differences for three consecutive rows */
static bool vert_ok[ROWS][ROWS][ROWS];            /* 32×32×32 */

/* For each (prev,current) pair we store all possible next rows */
static unsigned char nexts[ROWS * ROWS][ROWS];    /* 1024×32 */
static unsigned char next_cnt[ROWS * ROWS];       /* count for each pair */

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Handle the trivial index below the offset */
    if (n == 0) return 0;

    /* Build the tables on first call */
    if (!ready) {
        /* Compute pair_sum for every two rows */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                for (int j = 0; j < 4; ++j) {
                    int s = ((a >> j) & 1) + ((a >> (j + 1)) & 1)
                          + ((b >> j) & 1) + ((b >> (j + 1)) & 1);
                    pair_sum[a][b][j] = (unsigned char)s;
                }
            }
        }

        /* Horizontal constraint: for a single pair of rows */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    int diff = (int)pair_sum[a][b][j] -
                               (int)pair_sum[a][b][j + 1];
                    if (diff > 1 || diff < -1) {
                        ok = false;
                        break;
                    }
                }
                horiz_ok[a][b] = ok;
            }
        }

        /* Vertical constraint: for three consecutive rows */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                for (int c = 0; c < ROWS; ++c) {
                    bool ok = true;
                    for (int j = 0; j < 4; ++j) {
                        int diff = (int)pair_sum[a][b][j] -
                                   (int)pair_sum[b][c][j];
                        if (diff > 1 || diff < -1) {
                            ok = false;
                            break;
                        }
                    }
                    vert_ok[a][b][c] = ok;
                }
            }
        }

        /* Build adjacency lists: (prev,cur) -> list of next rows */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                int idx = a * ROWS + b;
                int cnt = 0;
                for (int c = 0; c < ROWS; ++c) {
                    if (horiz_ok[b][c] && vert_ok[a][b][c]) {
                        nexts[idx][cnt++] = (unsigned char)c;
                    }
                }
                next_cnt[idx] = (unsigned char)cnt;
            }
        }

        ready = true;
    }

    /* DP arrays: number of ways ending with row pair (prev,cur) */
    seq_t cur[ROWS][ROWS];
    seq_t nxt[ROWS][ROWS];

    /* initialise cur for the first two rows (n+1 = 2 rows when n = 1) */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            cur[i][j] = 0;

    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            if (horiz_ok[a][b]) {
                cur[a][b] = 1;
            }
        }
    }

    /* Add remaining rows (there are n-1 additional rows after the first two) */
    for (idx_t step = 0; step < n - 1; ++step) {

        /* zero nxt array */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                nxt[i][j] = 0;

        /* transition */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                seq_t val = cur[a][b];
                if (val == 0) continue;

                int idx = a * ROWS + b;
                unsigned int cnt = next_cnt[idx];
                unsigned char *list = nexts[idx];
                for (unsigned int k = 0; k < cnt; ++k) {
                    int c = list[k];
                    nxt[b][c] += val;
                }
            }
        }

        /* swap cur and nxt */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* Sum all states to get the total number of (n+1)×5 arrays */
    seq_t total = 0;
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            total += cur[a][b];

    /* Return half of that count, as required by the definition */
    return total / 2;
}
