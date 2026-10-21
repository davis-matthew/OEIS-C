/*
 * A232363  Number of n X 3 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or vertically.
 * https://oeis.org/A232363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP: counts the number of n×3 arrays of digits 0,1,2 such that
   every 0 has an adjacent 1 and every 1 has an adjacent 2
   (adjacency is horizontal or vertical).  The algorithm
   uses a transfer‑matrix over rows; for each row we keep a 3‑bit
   mask recording which cells already have a required neighbour
   (by left/right or the previous row).  A value of 0 in the mask
   means that the cell still needs to be satisfied by the next row.
   The initial row's mask depends only on horizontal neighbours,
   and at the end all cells must be satisfied (mask = 111b). */

#include <stdint.h>
#include <stdbool.h>

/* Precomputed tables for the 27 possible rows (base‑3 3‑digit numbers) */
static uint8_t row_digits[27][3];
static uint8_t req_val[27][3];            /* 1 for value 0, 2 for value 1, 0 for value 2 */
static uint8_t left_right_sat[27][3];     /* 1 if cell has required neighbour left/right */

/* One‑time initializer for the tables */
static void init_tables(void)
{
    static bool initialized = false;
    if (initialized) return;

    for (int r = 0; r < 27; ++r) {
        int d0 = r / 9;
        int d1 = (r / 3) % 3;
        int d2 = r % 3;
        row_digits[r][0] = (uint8_t)d0;
        row_digits[r][1] = (uint8_t)d1;
        row_digits[r][2] = (uint8_t)d2;

        for (int c = 0; c < 3; ++c) {
            uint8_t val = row_digits[r][c];
            if (val == 0) req_val[r][c] = 1;
            else if (val == 1) req_val[r][c] = 2;
            else req_val[r][c] = 0;                    /* val == 2 */

            bool sat = false;
            if (val == 2) {
                sat = true;                           /* 2 has no requirement */
            } else {
                if (c > 0 && row_digits[r][c - 1] == req_val[r][c]) sat = true;
                if (c < 2 && row_digits[r][c + 1] == req_val[r][c]) sat = true;
            }
            left_right_sat[r][c] = sat ? 1 : 0;
        }
    }
    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of defined range */
    init_tables();

    /* Number of possible states: 27 rows × 8 mask values */
    enum { STATE_VISITS = 27 * 8 };
    seq_t dp[STATE_VISITS] = {0};
    seq_t ndp[STATE_VISITS] = {0};

    /* ---------- First row ------------------------------------------------- */
    for (int r = 0; r < 27; ++r) {
        int mask = 0;
        for (int c = 0; c < 3; ++c) {
            if (row_digits[r][c] == 2) mask |= (1 << c);
            else if (left_right_sat[r][c]) mask |= (1 << c);
        }
        dp[r * 8 + mask] = (seq_t)1;
    }

    /* ---------- Subsequent rows ------------------------------------------ */
    for (idx_t row_no = 2; row_no <= n; ++row_no) {
        /* reset ndp */
        for (int i = 0; i < STATE_VISITS; ++i) ndp[i] = (seq_t)0;

        for (int prev_id = 0; prev_id < STATE_VISITS; ++prev_id) {
            seq_t ways = dp[prev_id];
            if (ways == (seq_t)0) continue;
            int prev_row = prev_id / 8;
            int prev_mask = prev_id % 8;

            for (int curr = 0; curr < 27; ++curr) {
                bool ok = true;

                /* Ensure that cells in the previous row that still lack a
                   satisfying neighbour receive it from the current row. */
                for (int c = 0; c < 3; ++c) {
                    if (prev_mask & (1 << c)) continue;   /* already satisfied */
                    int req = req_val[prev_row][c];       /* required value of prev cell */
                    if (req != row_digits[curr][c]) { ok = false; break; }
                }
                if (!ok) continue;

                /* Compute the new mask for the current row:
                   a bit is set if the cell already has a satisfying neighbour
                   either horizontally or by the previous row. */
                int new_mask = 0;
                for (int c = 0; c < 3; ++c) {
                    if (row_digits[curr][c] == 2) { new_mask |= (1 << c); continue; }
                    bool horiz = left_right_sat[curr][c];
                    bool vert  = (row_digits[prev_row][c] == req_val[curr][c]);
                    if (horiz || vert) new_mask |= (1 << c);
                }

                ndp[curr * 8 + new_mask] += ways;
            }
        }
        /* move to next iteration */
        for (int i = 0; i < STATE_VISITS; ++i) dp[i] = ndp[i];
    }

    /* ---------- Sum only fully satisfied final states --------------------- */
    seq_t result = (seq_t)0;
    for (int r = 0; r < 27; ++r) {
        result += dp[r * 8 + 7];   /* mask 111b means all cells satisfied */
    }
    return result;
}
