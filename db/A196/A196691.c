/*
 * A196691  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 2,1,3,0,4 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by row‑by‑row dynamic programming * * States: (row
 *            index, up_mask) * row index – integer from 0 to 124 representing
 *            a 3‑column row over {0..4} * up_mask – 3‑bit mask; bit c is 1
 *            iff the cell above the current cell * at column c equals the
 *            target value for the current cell. * * Trans...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define NUM_ROWS 125          /* 5^3 possible rows (3 columns, values 0..4) */
#define NUM_MASKS 8           /* 3‑bit mask for up‑neighbour matches */

/* Digit (base‑5) representation of each row */
static uint8_t row_val[NUM_ROWS][3];

/* Number of horizontal neighbors equal to the required target value
 * for each cell in each row. */
static uint8_t horiz_cnt[NUM_ROWS][3];

/* Target values for cell values 0..4 */
static const uint8_t target_val[5] = {2, 1, 3, 0, 4};

/* Transition tables:
 *   trans_next[prev][mask][k]   → next row index
 *   trans_newmask[prev][mask][k]→ new up_mask for the next row
 *   trans_count[prev][mask]     → number of valid transitions */
static uint8_t trans_next[NUM_ROWS][NUM_MASKS][NUM_ROWS];
static uint8_t trans_newmask[NUM_ROWS][NUM_MASKS][NUM_ROWS];
static unsigned char trans_count[NUM_ROWS][NUM_MASKS];

/* Final validity: final_valid[row][mask] == 1 iff
 * the last row configuration satisfies its constraints.
 */
static bool final_valid[NUM_ROWS][NUM_MASKS];

static bool tables_ready = false;

/* Prepare all tables once */
static void init_tables(void)
{
    if (tables_ready) return;
    tables_ready = true;

    /* Precompute digit representations of each row */
    for (int r = 0; r < NUM_ROWS; ++r) {
        int tmp = r;
        for (int c = 2; c >= 0; --c) {
            row_val[r][c] = tmp % 5;
            tmp /= 5;
        }
    }

    /* Precompute horizontal neighbor counts for each cell */
    for (int r = 0; r < NUM_ROWS; ++r) {
        for (int c = 0; c < 3; ++c) {
            uint8_t tv = target_val[row_val[r][c]];
            uint8_t h = 0;
            if (c > 0 && row_val[r][c-1] == tv) ++h;
            if (c < 2 && row_val[r][c+1] == tv) ++h;
            horiz_cnt[r][c] = h;
        }
    }

    /* Build transition tables */
    for (int prev = 0; prev < NUM_ROWS; ++prev) {
        for (int up_mask = 0; up_mask < NUM_MASKS; ++up_mask) {
            unsigned char count = 0;
            for (int next = 0; next < NUM_ROWS; ++next) {
                /* Check that prev row's constraints are satisfied
                   given up_mask and this next row. */
                bool ok = true;
                for (int c = 0; c < 3; ++c) {
                    uint8_t tv_prev = target_val[row_val[prev][c]];
                    uint8_t has_down = (row_val[next][c] == tv_prev) ? 1 : 0;
                    uint8_t up = ((up_mask >> c) & 1);
                    uint8_t total = horiz_cnt[prev][c] + up + has_down;
                    if (total != row_val[prev][c]) { ok = false; break; }
                }
                if (!ok) continue;

                /* Compute new up_mask for the next row */
                uint8_t new_mask = 0;
                for (int c = 0; c < 3; ++c) {
                    uint8_t tv_next = target_val[row_val[next][c]];
                    if (row_val[prev][c] == tv_next) new_mask |= (1 << c);
                }

                trans_next[prev][up_mask][count] = (uint8_t)next;
                trans_newmask[prev][up_mask][count] = new_mask;
                ++count;
            }
            trans_count[prev][up_mask] = count;
        }
    }

    /* Final validity table */
    for (int r = 0; r < NUM_ROWS; ++r) {
        for (int up_mask = 0; up_mask < NUM_MASKS; ++up_mask) {
            bool ok = true;
            for (int c = 0; c < 3; ++c) {
                uint8_t tv = target_val[row_val[r][c]];
                uint8_t up = ((up_mask >> c) & 1);
                uint8_t total = horiz_cnt[r][c] + up;
                if (total != row_val[r][c]) { ok = false; break; }
            }
            final_valid[r][up_mask] = ok;
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();

    /* Dynamic programming arrays */
    static seq_t dp[NUM_ROWS * NUM_MASKS];
    static seq_t dp_next[NUM_ROWS * NUM_MASKS];
    memset(dp, 0, sizeof(dp));

    /* Initial state: first row with no up neighbors */
    for (int r = 0; r < NUM_ROWS; ++r) {
        dp[ r * NUM_MASKS + 0 ] = (seq_t)1;
    }

    /* Process each subsequent row */
    for (idx_t step = 1; step < n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int prev = 0; prev < NUM_ROWS; ++prev) {
            for (int up_mask = 0; up_mask < NUM_MASKS; ++up_mask) {
                seq_t ways = dp[ prev * NUM_MASKS + up_mask ];
                if (!ways) continue;
                unsigned char cnt = trans_count[prev][up_mask];
                for (unsigned char t = 0; t < cnt; ++t) {
                    int next = trans_next[prev][up_mask][t];
                    int new_mask = trans_newmask[prev][up_mask][t];
                    dp_next[ next * NUM_MASKS + new_mask ] += ways;
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* Sum over final states */
    seq_t ans = 0;
    for (int r = 0; r < NUM_ROWS; ++r) {
        for (int up_mask = 0; up_mask < NUM_MASKS; ++up_mask) {
            if (final_valid[r][up_mask]) {
                ans += dp[ r * NUM_MASKS + up_mask ];
            }
        }
    }
    return ans;
}
