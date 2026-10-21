/*
 * A238924  Number of (n+1)X(2+1) 0..3 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A238924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over 0..3 with the property that
 *            no element is larger than all its horizontal neighbours and no
 *            element is equal to all its vertical neighbours. The array is
 *            counted for every position; horizontal neighbours are
 *            left/right, vertical neighbours are up/down. The rule...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>  /* for memset */

#define ROWS_COUNT 16          /* number of rows satisfying horizontal rules */
#define COLS      3

/* Store the vertically valid rows, the all‑column‑different table,
   and the transition lists for the DP.  All data structures are
   built once on the first call to a(). */
static uint8_t  row_val[ROWS_COUNT][COLS];
static uint8_t  next_idx[ROWS_COUNT][ROWS_COUNT][ROWS_COUNT];
static uint8_t  next_cnt[ROWS_COUNT][ROWS_COUNT];
static uint8_t  agree_col[ROWS_COUNT][ROWS_COUNT];     /* 1 iff rows differ in all 3 columns */

static void build_data(void)
{
    /* --- generate all rows that satisfy the horizontal constraints --- */
    int rc = 0;                      /* number of good rows found */

    for (int code = 0; code < 64; ++code) {
        uint8_t a = (code >> 0) & 3;
        uint8_t b = (code >> 2) & 3;
        uint8_t c = (code >> 4) & 3;

        /* horizontal rules:
           left <= middle, right <= middle, and middle not strictly greater
           than both neighbours. */
        if (a <= b && c <= b && (b <= a || b <= c)) {
            row_val[rc][0] = a;
            row_val[rc][1] = b;
            row_val[rc][2] = c;
            ++rc;
        }
    }   /* rc should be 16 */

    /* --- pre‑compute all‑column‑different table for top/bottom edges --- */
    for (int i = 0; i < rc; ++i) {
        for (int j = 0; j < rc; ++j) {
            agree_col[i][j] =
                (row_val[i][0] != row_val[j][0] &&
                 row_val[i][1] != row_val[j][1] &&
                 row_val[i][2] != row_val[j][2]) ? 1 : 0;
        }
    }

    /* --- pre‑compute transition lists (prev , middle) -> list of next rows --- */
    for (int p = 0; p < rc; ++p) {
        for (int m = 0; m < rc; ++m) {
            int cnt = 0;
            for (int n = 0; n < rc; ++n) {
                int all_equal = 0;
                for (int k = 0; k < COLS; ++k) {
                    if (row_val[m][k] == row_val[p][k] &&
                        row_val[m][k] == row_val[n][k]) {
                        all_equal = 1;
                        break;
                    }
                }
                if (!all_equal) {
                    next_idx[p][m][cnt] = (uint8_t)n;
                    ++cnt;
                }
            }
            next_cnt[p][m] = (uint8_t)cnt;
        }
    }
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* offset 1 is the first term */
    if (n < 1) return 0;

    /* initialise tables on the first call */
    static int initialized = 0;
    if (!initialized) {
        build_data();
        initialized = 1;
    }

    const int  R = ROWS_COUNT;          /* fixed 16 */
    /* DP buffers */
    static seq_t dp[ROWS_COUNT][ROWS_COUNT];
    static seq_t dp_next[ROWS_COUNT][ROWS_COUNT];
    static seq_t tmp[ROWS_COUNT][ROWS_COUNT];

    /* number of rows in the array */
    idx_t rows = n + 1;        /* L = n+1 */
    idx_t remaining = rows - 2;   /* number of transitions after the first two rows */

    /* initialise dp with all top‑bottom edge pairs that differ in every column */
    memset(dp, 0, sizeof(dp));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < R; ++j) {
            if (agree_col[i][j]) dp[i][j] = (seq_t)1;
        }
    }

    /* iterate over remaining rows */
    for (idx_t step = 0; step < remaining; ++step) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int p = 0; p < R; ++p) {
            for (int m = 0; m < R; ++m) {
                seq_t cur = dp[p][m];
                if (!cur) continue;
                int cnt = next_cnt[p][m];
                for (int t = 0; t < cnt; ++t) {
                    int nrow = next_idx[p][m][t];
                    dp_next[m][nrow] += cur;
                }
            }
        }

        /* rotate buffers */
        memcpy(tmp, dp, sizeof(tmp));
        memcpy(dp, dp_next, sizeof(dp));
        memcpy(dp_next, tmp, sizeof(tmp));
    }

    /* sum up all valid bottom‑edge states */
    seq_t total = (seq_t)0;
    for (int p = 0; p < R; ++p) {
        for (int c = 0; c < R; ++c) {
            if (!agree_col[p][c]) continue;   /* bottom row must differ in all columns */
            total += dp[p][c];
        }
    }

    return total;
}
