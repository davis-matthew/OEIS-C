/*
 * A237846  Number of (n+1)X(4+1) 0..2 arrays with the maximum minus the lower
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays with entries in {0,1,2} such
 *            that for every 2×2 subblock the value (maximum – lower median)
 *            is the same constant. No closed‑form formula is known, so we
 *            count all arrays by dynamic programming over the rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   block_has_d, lower_median_of_block, max_of_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/block_has_d.h"
#include "../utils/lower_median_of_block.h"
#include "../utils/max_of_block.h"

/* Pre‑computed data */
static uint8_t rows[243][5];
static int    neigh_cnt[3][243];
static int    neigh[3][243][243];
static bool   initialized = false;

/* One‑time initialization of rows and adjacency lists */
static void initialize(void)
{
    if (initialized) return;

    /* enumerate all 3^5 = 243 possible rows */
    for (int idx = 0; idx < 243; ++idx) {
        int tmp = idx;
        for (int col = 0; col < 5; ++col) {
            rows[idx][col] = tmp % 3;
            tmp /= 3;
        }
    }

    /* build adjacency lists for each difference d */
    for (int d = 0; d < 3; ++d) {
        for (int i = 0; i < 243; ++i) {
            neigh_cnt[d][i] = 0;
            for (int j = 0; j < 243; ++j) {
                if (block_has_d(d, rows[i], rows[j])) {
                    neigh[d][i][neigh_cnt[d][i]++] = j;
                }
            }
        }
    }
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    initialize();

    if (n == 0) return 0;   /* offset is 1 */

    size_t steps = (size_t)n;   /* number of transitions (rows-1) */
    seq_t total = 0;

    /* two DP layers */
    static seq_t dp_cur_arr[243];
    static seq_t dp_next_arr[243];
    seq_t *dp_cur  = dp_cur_arr;
    seq_t *dp_next = dp_next_arr;

    for (int d = 0; d < 3; ++d) {
        /* start with any first row */
        for (int i = 0; i < 243; ++i) dp_cur[i] = 1;

        for (size_t step = 0; step < steps; ++step) {
            for (int i = 0; i < 243; ++i) dp_next[i] = 0;

            for (int prev = 0; prev < 243; ++prev) {
                seq_t val = dp_cur[prev];
                int cnt   = neigh_cnt[d][prev];
                int *list = neigh[d][prev];
                for (int k = 0; k < cnt; ++k) {
                    int next = list[k];
                    dp_next[next] += val;
                }
            }
            /* swap layers for next step */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }

        /* sum the counts for this difference d */
        seq_t cnt_d = 0;
        for (int i = 0; i < 243; ++i) cnt_d += dp_cur[i];
        total += cnt_d;
    }

    return total;
}
