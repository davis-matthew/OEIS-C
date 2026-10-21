/*
 * A233678  Number of (n+1) X (4+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 6.
 * https://oeis.org/A233678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 5 arrays over {0,1,2,3} such that every
 *            2×2 subblock has the sum of the squares of the edge differences
 *            equal to 6. The sequence counts the number of such arrays for n
 *            ≥ 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROW_COUNT 1024              /* 4^5 possible rows */
#define MAX_DEGREE 64               /* generous upper bound on adjacency list size */

/* adjacency data */
static unsigned short adj_list[ROW_COUNT][MAX_DEGREE];
static unsigned char  adj_cnt[ROW_COUNT];

/* row digit extraction cache */
static unsigned char row_digits[ROW_COUNT][5];

/* adjacency initialization flag */
static bool adj_inited = false;

/* helper to initialize adjacency lists */
static void init_adjacency(void)
{
    if (adj_inited) return;
    adj_inited = true;

    /* Precompute each row's 5 base-4 digits */
    for (idx_t r = 0; r < ROW_COUNT; ++r) {
        for (int c = 0; c < 5; ++c) {
            row_digits[r][c] = (unsigned char)((r >> (2 * c)) & 3);
        }
    }

    /* Precompute which 2×2 blocks are valid */
    bool good[256];
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                for (int d = 0; d < 4; ++d) {
                    int sum = (b - a) * (b - a) +
                              (c - a) * (c - a) +
                              (d - b) * (d - b) +
                              (d - c) * (d - c);
                    good[(a << 6) | (b << 4) | (c << 2) | d] = (sum == 6);
                }

    /* Build adjacency lists: i → j if every 2×2 subblock between rows i and j is good */
    for (idx_t i = 0; i < ROW_COUNT; ++i) {
        adj_cnt[i] = 0;
        for (idx_t j = 0; j < ROW_COUNT; ++j) {
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                unsigned char a = row_digits[i][col];
                unsigned char b = row_digits[i][col + 1];
                unsigned char c = row_digits[j][col];
                unsigned char d = row_digits[j][col + 1];
                if (!good[(a << 6) | (b << 4) | (c << 2) | d]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                /* Store adjacency; MAX_DEGREE is ample for the detected sparsity */
                adj_list[i][adj_cnt[i]] = (unsigned short)j;
                ++adj_cnt[i];
            }
        }
    }
}

/* DP state */
static seq_t dp_cur[ROW_COUNT];
static seq_t dp_next[ROW_COUNT];

/* Cached totals: totals[n] = a(n) for n >= 1 */
static seq_t *totals = NULL;
static idx_t  totals_len = 0;     /* number of allocated entries, indexed from 0 */
static idx_t  max_n_computed = 0;

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Ensure adjacency lists are built */
    init_adjacency();

    /* Allocate or grow totals array if needed */
    if (totals_len < n + 2) {
        idx_t new_len = n + 2;
        seq_t *new_totals = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (totals) {
            memcpy(new_totals, totals, totals_len * sizeof(seq_t));
            free(totals);
        }
        totals = new_totals;
        totals_len = new_len;
    }

    /* First time call: initialise dp_cur to all 1's and compute up to n */
    if (max_n_computed == 0) {
        for (idx_t i = 0; i < ROW_COUNT; ++i) dp_cur[i] = (seq_t)1;

        for (idx_t step = 1; step <= n; ++step) {
            /* zero dp_next */
            memset(dp_next, 0, ROW_COUNT * sizeof(seq_t));

            /* Transition */
            for (idx_t i = 0; i < ROW_COUNT; ++i) {
                seq_t val = dp_cur[i];
                if (val == 0) continue;
                unsigned char deg = adj_cnt[i];
                for (unsigned char k = 0; k < deg; ++k) {
                    unsigned short j = adj_list[i][k];
                    dp_next[j] += val;
                }
            }

            /* Sum to get a(step) */
            seq_t sum = 0;
            for (idx_t i = 0; i < ROW_COUNT; ++i) sum += dp_next[i];
            totals[step] = sum;

            /* Move dp_next to dp_cur for next iteration */
            memcpy(dp_cur, dp_next, ROW_COUNT * sizeof(seq_t));
        }
        max_n_computed = n;
        return totals[n];
    }

    /* If we already have computed up to n, return result directly */
    if (n <= max_n_computed)
        return totals[n];

    /* Compute from previous maximum up to the new n */
    for (idx_t step = max_n_computed + 1; step <= n; ++step) {
        memset(dp_next, 0, ROW_COUNT * sizeof(seq_t));

        for (idx_t i = 0; i < ROW_COUNT; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            unsigned char deg = adj_cnt[i];
            for (unsigned char k = 0; k < deg; ++k) {
                unsigned short j = adj_list[i][k];
                dp_next[j] += val;
            }
        }

        seq_t sum = 0;
        for (idx_t i = 0; i < ROW_COUNT; ++i) sum += dp_next[i];
        totals[step] = sum;

        memcpy(dp_cur, dp_next, ROW_COUNT * sizeof(seq_t));
    }
    max_n_computed = n;
    return totals[n];
}
