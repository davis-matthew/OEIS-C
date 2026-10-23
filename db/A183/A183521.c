/*
 * A183521  Number of nX4 0..2 arrays with each element equal to either the
 *          sum mod 3 of its horizontal and vertical neighbors or the sum mod
 *          3 of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Computation:  DP over rows.
   For each n×4 array over {0,1,2} each cell must equal either
   the sum mod 3 of its horizontal and vertical neighbours
   or the sum mod 3 of its diagonal and anti‑diagonal neighbours.
   The function counts all such arrays. */

/* Number of possible 4‑cell rows over {0,1,2} */
#define ROWS 81                 /* 3^4 = 81 */
#define ZERO_ROW 0             /* index of all‑zero row */

/* Row values encoded as base‑3 digits */
static uint8_t row_vals[ROWS][4];

/* Pre‑computed validity of (prev_row, cur_row, next_row) triples */
static bool valid_trip[ROWS][ROWS][ROWS];

/* For every pair (prev,cur) store the list of next rows that form a valid triple */
static uint8_t next_list[ROWS][ROWS][ROWS];
static uint8_t next_count[ROWS][ROWS];

/* ------------------------------------------------------------------ */
/* Encode a 4‑cell row (base‑3 digits) into an index 0..80 */
static inline int encode_row(uint8_t v[4])
{
    return v[0] + 3 * (v[1] + 3 * (v[2] + 3 * v[3]));
}

/* ------------------------------------------------------------------ */
/* Pre‑computation of all rows and triples */
static void precompute(void)
{
    /* Build all rows */
    for (int idx = 0; idx < ROWS; ++idx) {
        int tmp = idx;
        for (int j = 0; j < 4; ++j) {
            row_vals[idx][j] = tmp % 3;
            tmp /= 3;
        }
    }

    /* Validate triples and build transition lists */
    for (int p = 0; p < ROWS; ++p) {
        const uint8_t *prev = row_vals[p];
        for (int c = 0; c < ROWS; ++c) {
            const uint8_t *mid = row_vals[c];
            uint8_t cnt = 0;
            for (int n = 0; n < ROWS; ++n) {
                const uint8_t *next = row_vals[n];
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    /* Horizontal + vertical sum */
                    int hv = 0;
                    if (j > 0)     hv += mid[j - 1];
                    if (j < 3)     hv += mid[j + 1];
                    hv += prev[j];
                    hv += next[j];
                    hv %= 3;

                    /* Diagonal + anti‑diagonal sum */
                    int da = 0;
                    if (j > 0)     da += prev[j - 1];
                    if (j < 3)     da += prev[j + 1];
                    if (j > 0)     da += next[j - 1];
                    if (j < 3)     da += next[j + 1];
                    da %= 3;

                    if (mid[j] != hv && mid[j] != da) {
                        ok = false;
                    }
                }
                valid_trip[p][c][n] = ok;
                if (ok) {
                    next_list[p][c][cnt] = (uint8_t)n;
                    cnt++;
                }
            }
            next_count[p][c] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise precomputed data on first call */
    static bool inited = false;
    if (!inited) {
        precompute();
        inited = true;
    }

    /* DP arrays */
    static seq_t dp_cur[ROWS * ROWS];
    static seq_t dp_next[ROWS * ROWS];
    memset(dp_cur, 0, sizeof(dp_cur));
    memset(dp_next, 0, sizeof(dp_next));

    /* initial state: zero row above first row */
    for (int cur = 0; cur < ROWS; ++cur) {
        dp_cur[ZERO_ROW * ROWS + cur] = (seq_t)1;
    }

    if (n == 1) {
        /* only one row: check validity with zero rows above and below */
        seq_t total = 0;
        for (int cur = 0; cur < ROWS; ++cur) {
            if (valid_trip[ZERO_ROW][cur][ZERO_ROW]) {
                total += dp_cur[ZERO_ROW * ROWS + cur];
            }
        }
        return total;
    }

    /* process rows 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                seq_t val = dp_cur[prev * ROWS + cur];
                if (val == 0) continue;
                uint8_t cnt = next_count[prev][cur];
                const uint8_t *list = next_list[prev][cur];
                for (uint8_t k = 0; k < cnt; ++k) {
                    int nxt = list[k];
                    dp_next[cur * ROWS + nxt] += val;
                }
            }
        }
        /* move dp_next to dp_cur for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* final check: last row must satisfy constraint with zero row below */
    seq_t total = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            if (valid_trip[prev][cur][ZERO_ROW]) {
                total += dp_cur[prev * ROWS + cur];
            }
        }
    }
    return total;
}
