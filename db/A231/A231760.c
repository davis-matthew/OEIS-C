/*
 * A231760  Number of (n+1)X(4+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 binary arrays with no element having a
 *            strict majority of its horizontal, diagonal and antidiagonal
 *            neighbors equal to one. Computed via dynamic programming over
 *            rows
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Precomputed tables for validity checks */
static bool initialized = false;
static bool valid_top[32][32];
static bool valid_mid[32][32][32];
static uint8_t next_list[32][32][32];
static uint8_t next_count[32][32];

/* Build the validity tables once */
static void precompute(void)
{
    if (initialized) return;

    /* Validity for the top row (only below neighbor exists) */
    for (int b = 0; b < 32; ++b) {
        for (int n = 0; n < 32; ++n) {
            bool ok = true;
            for (int col = 0; col < 5 && ok; ++col) {
                int k = 0, ones = 0;
                if (col > 0) { k++; ones += (b >> (col - 1)) & 1; }
                if (col < 4) { k++; ones += (b >> (col + 1)) & 1; }
                if (col > 0) { k++; ones += (n >> (col - 1)) & 1; }
                if (col < 4) { k++; ones += (n >> (col + 1)) & 1; }
                if ((ones << 1) > k) { ok = false; break; }
            }
            valid_top[b][n] = ok;
        }
    }

    /* Validity for a middle row (above and below exist) */
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            for (int c = 0; c < 32; ++c) {
                bool ok = true;
                for (int col = 0; col < 5 && ok; ++col) {
                    int k = 0, ones = 0;
                    if (col > 0) { k++; ones += (b >> (col - 1)) & 1; }
                    if (col < 4) { k++; ones += (b >> (col + 1)) & 1; }
                    if (col > 0) { k++; ones += (a >> (col - 1)) & 1; }
                    if (col < 4) { k++; ones += (a >> (col + 1)) & 1; }
                    if (col > 0) { k++; ones += (c >> (col - 1)) & 1; }
                    if (col < 4) { k++; ones += (c >> (col + 1)) & 1; }
                    if ((ones << 1) > k) { ok = false; break; }
                }
                valid_mid[a][b][c] = ok;
                if (ok) {
                    int idx = next_count[a][b];
                    next_list[a][b][idx] = c;
                    next_count[a][b] = idx + 1;
                }
            }
        }
    }

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    precompute();

    idx_t total_rows = n + 1;          /* number of rows in the array */
    seq_t dp[32][32];
    seq_t newdp[32][32];

    /* Initialize DP with the first two rows */
    for (int r = 0; r < 32; ++r)
        for (int s = 0; s < 32; ++s)
            dp[r][s] = 0;

    for (int r0 = 0; r0 < 32; ++r0) {
        for (int r1 = 0; r1 < 32; ++r1) {
            if (valid_top[r0][r1])
                dp[r0][r1] += (seq_t)1;
        }
    }

    /* Add rows 2 .. total_rows-1 */
    for (idx_t i = 2; i < total_rows; ++i) {
        for (int r = 0; r < 32; ++r)
            for (int s = 0; s < 32; ++s)
                newdp[r][s] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == (seq_t)0) continue;
                uint8_t cnt = next_count[prev][curr];
                for (int t = 0; t < cnt; ++t) {
                    int nxt = next_list[prev][curr][t];
                    newdp[curr][nxt] += val;
                }
            }
        }

        /* copy newdp to dp */
        for (int r = 0; r < 32; ++r)
            for (int s = 0; s < 32; ++s)
                dp[r][s] = newdp[r][s];
    }

    /* Final check for the bottom row */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t val = dp[prev][curr];
            if (val == (seq_t)0) continue;
            if (valid_top[curr][prev]) /* bottom row uses the same test */
                ans += val;
        }
    }

    return ans;
}
