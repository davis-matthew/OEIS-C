/*
 * A236906  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the upper
 *          median plus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A236906:
   a(n) = number of (n+1) × 4 arrays with entries 0..3 such that
   the quantity max + upper‑median + min of every 2×2 subblock
   is equal for all subblocks. */

/* Note: idx_t and seq_t are defined elsewhere by the build system. */
#include <stdbool.h>

#define ROWS      256     /* 4^4 possible rows of length 4 */
#define S_VALUES   10     /* possible sums 0..9 */

static uint8_t  g_row_digits[ROWS][4];
static int8_t   g_pairS[ROWS][ROWS];
static uint8_t  g_compat[ROWS][S_VALUES][ROWS]; /* adjacency lists */
static uint8_t  g_compat_len[ROWS][S_VALUES];
static bool     g_init_done = false;

/* Precompute all tables once. */
static void init_precomp(void)
{
    /* Encode each row as four base‑4 digits. */
    for (idx_t r = 0; r < ROWS; ++r) {
        idx_t tmp = r;
        for (int c = 3; c >= 0; --c) {
            g_row_digits[r][c] = (uint8_t)(tmp & 3U);
            tmp >>= 2;
        }
    }

    /* Compute pairS[r1][r2]:  sum for the pair if compatible, else -1. */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int s = -1;
            for (int j = 0; j < 3; ++j) {
                uint8_t a = g_row_digits[r1][j];
                uint8_t b = g_row_digits[r1][j + 1];
                uint8_t c = g_row_digits[r2][j];
                uint8_t d = g_row_digits[r2][j + 1];

                /* Sort the four values. */
                uint8_t arr[4] = {a, b, c, d};
                for (int p = 0; p < 3; ++p)
                    for (int q = p + 1; q < 4; ++q)
                        if (arr[p] > arr[q]) {
                            uint8_t t = arr[p];
                            arr[p] = arr[q];
                            arr[q] = t;
                        }

                int sum = arr[0] + arr[2] + arr[3]; /* min + upper median + max */

                if (j == 0)
                    s = sum;
                else if (sum != s) {
                    s = -1;            /* incompatible */
                    break;
                }
            }
            g_pairS[r1][r2] = (int8_t)(s >= 0 ? s : -1);
        }
    }

    /* Build adjacency lists by (previous_row, S). */
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int s = 0; s < S_VALUES; ++s) {
            uint8_t len = 0;
            for (int curr = 0; curr < ROWS; ++curr) {
                if (g_pairS[prev][curr] == s) {
                    g_compat[prev][s][len++] = (uint8_t)curr;
                }
            }
            g_compat_len[prev][s] = len;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!g_init_done) {
        init_precomp();
        g_init_done = true;
    }

    if (n == 0) {            /* offset is 1, not used for n=0 */
        return (seq_t)0;
    }

    /* Number of rows in the array: n+1 */
    const idx_t rows = n + 1;

    /* dp[S][row] holds counts of arrays of current height
       ending with current row, with common sum S. */
    seq_t dp[S_VALUES][ROWS];
    seq_t newdp[S_VALUES][ROWS];

    /* Initialise dp for pairs of the first two rows. */
    for (int s = 0; s < S_VALUES; ++s)
        for (int r = 0; r < ROWS; ++r)
            dp[s][r] = 0;

    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int8_t s = g_pairS[r0][r1];
            if (s >= 0)
                dp[(int)s][r1] += (seq_t)1;
        }
    }

    /* Extend the array row by row. */
    for (idx_t step = 1; step < rows - 1; ++step) {
        /* Clear newdp */
        for (int s = 0; s < S_VALUES; ++s)
            for (int r = 0; r < ROWS; ++r)
                newdp[s][r] = 0;

        /* Transition from previous row to current row. */
        for (int s = 0; s < S_VALUES; ++s) {
            for (int prev = 0; prev < ROWS; ++prev) {
                seq_t cnt = dp[s][prev];
                if (cnt == (seq_t)0) continue;
                uint8_t len = g_compat_len[prev][s];
                for (uint8_t k = 0; k < len; ++k) {
                    uint8_t curr = g_compat[prev][s][k];
                    newdp[s][curr] += cnt;
                }
            }
        }

        /* Move newdp into dp for next iteration. */
        for (int s = 0; s < S_VALUES; ++s)
            for (int r = 0; r < ROWS; ++r)
                dp[s][r] = newdp[s][r];
    }

    /* Sum all counts: each represents a distinct array. */
    seq_t ans = 0;
    for (int s = 0; s < S_VALUES; ++s)
        for (int r = 0; r < ROWS; ++r)
            ans += dp[s][r];

    return ans;
}
