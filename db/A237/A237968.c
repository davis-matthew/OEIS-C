/*
 * A237968  Number of (n+1)X(2+1) 0..3 arrays with the upper median of every
 *          2X2 subblock differing from its horizontal and vertical neighbors
 *          by exactly one.
 * https://oeis.org/A237968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   median4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237968: number of (n+1)×3 arrays with entries 0..3 whose upper median
   of every 2×2 subblock differs from its horizontal and vertical
   neighbours by exactly one.  Computed by dynamic programming on
   consecutive row pairs. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/median4.h"

/* ---- pair of consecutive rows with local medians ---- */
struct Pair {
    uint8_t row_prev;   /* encoded 3‑cell row (0..63) */
    uint8_t row_curr;   /* encoded 3‑cell row (0..63) */
    uint8_t m0;         /* upper median of columns 0–1 */
    uint8_t m1;         /* upper median of columns 1–2 */
};

/* ---- static data for DP ---- */
static bool init_done = false;
static struct Pair *pair_data   = NULL;
static int          pair_count  = 0;
static int          list_len[64] = {0};
static int         *list_ptr[64] = {NULL};

/* ---- construction of all valid pairs and transition lists ---- */
static void init_pairs(void)
{
    if (init_done) return;

    /* temporary storage for at most 64×64 pairs */
    struct Pair temp[4096];
    int temp_cnt = 0;

    for (int r1 = 0; r1 < 64; ++r1) {
        uint8_t a = (r1 >> 4) & 3;
        uint8_t b = (r1 >> 2) & 3;
        uint8_t c = r1 & 3;
        for (int r2 = 0; r2 < 64; ++r2) {
            uint8_t d = (r2 >> 4) & 3;
            uint8_t e = (r2 >> 2) & 3;
            uint8_t f = r2 & 3;

            uint8_t m0 = median4(a, b, d, e);
            uint8_t m1 = median4(b, c, e, f);

            if ((m0 > m1 ? m0 - m1 : m1 - m0) == 1) {
                temp[temp_cnt].row_prev = (uint8_t)r1;
                temp[temp_cnt].row_curr = (uint8_t)r2;
                temp[temp_cnt].m0      = m0;
                temp[temp_cnt].m1      = m1;
                ++temp_cnt;
            }
        }
    }

    pair_count = temp_cnt;
    pair_data  = (struct Pair *)malloc(pair_count * sizeof(struct Pair));
    for (int i = 0; i < pair_count; ++i) pair_data[i] = temp[i];

    /* compute outgoing list lengths */
    for (int i = 0; i < pair_count; ++i)
        ++list_len[pair_data[i].row_prev];

    for (int r = 0; r < 64; ++r) {
        if (list_len[r] > 0)
            list_ptr[r] = (int *)malloc(list_len[r] * sizeof(int));
    }

    /* fill the lists */
    int idx[64] = {0};
    for (int i = 0; i < pair_count; ++i) {
        uint8_t r = pair_data[i].row_prev;
        list_ptr[r][idx[r]++] = i;
    }

    init_done = true;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_pairs();

    /* DP arrays */
    seq_t *dp_cur  = (seq_t *)calloc(pair_count, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(pair_count, sizeof(seq_t));

    /* base: each pair is a valid start */
    for (int i = 0; i < pair_count; ++i) dp_cur[i] = (seq_t)1;

    /* transitions */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < pair_count; ++i) dp_next[i] = 0;

        for (int p = 0; p < pair_count; ++p) {
            seq_t ways = dp_cur[p];
            if (ways == (seq_t)0) continue;

            uint8_t cur_row = pair_data[p].row_curr;
            int    len      = list_len[cur_row];
            int   *lst      = list_ptr[cur_row];

            for (int li = 0; li < len; ++li) {
                int q = lst[li];

                /* vertical neighbor constraints */
                int diff0 = (int)pair_data[p].m0 - (int)pair_data[q].m0;
                if (diff0 < 0) diff0 = -diff0;
                if (diff0 != 1) continue;

                int diff1 = (int)pair_data[p].m1 - (int)pair_data[q].m1;
                if (diff1 < 0) diff1 = -diff1;
                if (diff1 != 1) continue;

                dp_next[q] = dp_next[q] + ways;
            }
        }

        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all counts */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < pair_count; ++i)
        ans = ans + dp_cur[i];

    free(dp_cur);
    free(dp_next);
    return ans;
}
