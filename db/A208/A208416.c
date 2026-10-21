/*
 * A208416  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A208416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 47, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * OEIS A208416
 * a(n) = number of n×4 {0,1}-arrays avoiding horizontal 000 and 001
 *        and vertical 000 and 011.
 *
 * The values are computed by dynamic programming over rows.
 * The initial values are
 *    a(1) = 10
 *    a(2) = 100
 *
 * For n ≥ 3 we keep a state of the last two rows.
 * For each pair (p2,p1) we pre‑compute the admissible next rows r
 * that satisfy the vertical constraints in every column.
 * In addition each row r must satisfy the horizontal constraint
 * (no 000 or 001 in the row).  This ensures that the recurrence
 * matches the OEIS data exactly.
 */

#include <stdint.h>
#include <stdbool.h>

/* Number of possible 4‑bit rows (16). */
#define ROWS 16

/* Flags for rows that satisfy the horizontal rule. */
static bool horizontal_ok[ROWS];

/* For each pair (p2,p1) we store all admissible next rows. */
static unsigned char trans_next[ROWS * ROWS][ROWS];
static unsigned char trans_cnt[ROWS * ROWS];

/* Flag indicating whether the tables are initialised. */
static bool initialized = false;

static void init_tables(void)
{
    /* Horizontal constraint: no 000 or 001 in any 3‑bit window. */
    for (int m = 0; m < ROWS; ++m) {
        bool ok = true;
        /* two windows of length 3 in a 4‑bit row */
        for (int col = 0; col <= 1; ++col) {
            int b0 = (m >> col) & 1;
            int b1 = (m >> (col + 1)) & 1;
            int b2 = (m >> (col + 2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                (b0 == 0 && b1 == 0 && b2 == 1)) {
                ok = false;
                break;
            }
        }
        horizontal_ok[m] = ok;
    }

    /* Vertical transitions: avoid 000 and 011 in any column for
       the triple (p2,p1,r).  Only rows r that also satisfy the
       horizontal rule are kept. */
    for (int p2 = 0; p2 < ROWS; ++p2) {
        for (int p1 = 0; p1 < ROWS; ++p1) {
            int idx = (p2 << 4) | p1;
            unsigned char cnt = 0;
            for (int r = 0; r < ROWS; ++r) {
                if (!horizontal_ok[r]) continue;     /* r must be horizontal OK */
                bool ok = true;
                for (int col = 0; col < 4; ++col) {
                    int b2 = (p2 >> col) & 1;
                    int b1 = (p1 >> col) & 1;
                    int b0 = (r >> col) & 1;
                    if ((b2 == 0 && b1 == 0 && b0 == 0) ||
                        (b2 == 0 && b1 == 1 && b0 == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    trans_next[idx][cnt++] = (unsigned char)r;
                }
            }
            trans_cnt[idx] = cnt;
        }
    }
    initialized = true;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* a(0) is undefined */

    if (!initialized) init_tables();

    /* Count of valid single rows. */
    seq_t hcnt = 0;
    for (int m = 0; m < ROWS; ++m)
        if (horizontal_ok[m]) hcnt++;

    if (n == 1) return hcnt;             /* a(1) */

    /* DP tables: dp holds counts for each pair (p2,p1). */
    seq_t dp[ROWS * ROWS] = {0};
    seq_t newdp[ROWS * ROWS] = {0};

    /* Initialise for rows 1 and 2: all horizontal‑valid pairs are allowed. */
    for (int p2 = 0; p2 < ROWS; ++p2)
        if (horizontal_ok[p2])
            for (int p1 = 0; p1 < ROWS; ++p1)
                if (horizontal_ok[p1])
                    dp[(p2 << 4) | p1] = 1;

    /* Add rows 3 … n. */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < ROWS * ROWS; ++i) newdp[i] = 0;

        for (int idx = 0; idx < ROWS * ROWS; ++idx) {
            seq_t ways = dp[idx];
            if (!ways) continue;
            int p1   = idx & 15;                 /* lower 4 bits = row p1 */
            unsigned char cnt = trans_cnt[idx];
            for (unsigned char t = 0; t < cnt; ++t) {
                int r = trans_next[idx][t];
                int newIdx = (p1 << 4) | r;
                newdp[newIdx] += ways;
            }
        }
        /* copy newdp to dp */
        for (int i = 0; i < ROWS * ROWS; ++i) dp[i] = newdp[i];
    }

    /* Sum all ending states. */
    seq_t total = 0;
    for (int i = 0; i < ROWS * ROWS; ++i)
        total += dp[i];

    return total;
}
