/*
 * A219869  Number of tilings of a 6 X n rectangle using dominoes and straight
 *          (3 X 1) trominoes.
 * https://oeis.org/A219869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A219869: a(n) = Number of tilings of a 6×n rectangle using dominoes and straight (3×1) trominoes */
#include <stddef.h>
#include <stdint.h>

#define FULL_MASK 0x3F            /* 6 bits all set */
#define STATE_BITS 6
#define STATE_SIZE (1 << (2 * STATE_BITS))   /* 12‑bit states → 4096 */

/* Pre‑order DFS that fills the current column.  cur_mask holds the occupancy
   of the current column, nxt_mask holds the occupancy of the next column
   that is already filled, and nxt2_mask holds the occupancy of the column
   after that.  add is the number of ways to reach the current state. */
static void dfs_fill(int cur_mask, int nxt_mask, int nxt2_mask,
                     seq_t add,
                     seq_t *nxt_dp, uint32_t *nxt_act, size_t *nxt_len)
{
    /* All cells in the current column are occupied: finish this step */
    if (cur_mask == FULL_MASK) {
        uint32_t idx = ((nxt_mask & FULL_MASK) << STATE_BITS) |
                       (nxt2_mask & FULL_MASK);
        if (nxt_dp[idx] == 0) {
            nxt_act[(*nxt_len)++] = idx;
        }
        nxt_dp[idx] += add;
        return;
    }

    /* Find first free row in the current column */
    int r;
    for (r = 0; (cur_mask >> r) & 1; ++r);

    /* Place a vertical domino (2×1) if possible */
    if (r < 5 && ((cur_mask >> (r + 1)) & 1) == 0) {
        int m = cur_mask | (1 << r) | (1 << (r + 1));
        dfs_fill(m, nxt_mask, nxt2_mask, add, nxt_dp, nxt_act, nxt_len);
    }

    /* Place a vertical tromino (3×1) if possible */
    if (r < 4 &&
        ((cur_mask >> (r + 1)) & 1) == 0 &&
        ((cur_mask >> (r + 2)) & 1) == 0) {
        int m = cur_mask | (1 << r) | (1 << (r + 1)) | (1 << (r + 2));
        dfs_fill(m, nxt_mask, nxt2_mask, add, nxt_dp, nxt_act, nxt_len);
    }

    /* Place a horizontal domino (1×2) if possible */
    if (((nxt_mask >> r) & 1) == 0) {
        dfs_fill(cur_mask | (1 << r),
                 nxt_mask | (1 << r), nxt2_mask,
                 add, nxt_dp, nxt_act, nxt_len);
    }

    /* Place a horizontal tromino (1×3) if possible */
    if (((nxt_mask >> r) & 1) == 0 &&
        ((nxt2_mask >> r) & 1) == 0) {
        dfs_fill(cur_mask | (1 << r),
                 nxt_mask | (1 << r), nxt2_mask | (1 << r),
                 add, nxt_dp, nxt_act, nxt_len);
    }
}

seq_t a(idx_t n)
{
    static seq_t dp0[STATE_SIZE], dp1[STATE_SIZE];
    static uint32_t act0[STATE_SIZE], act1[STATE_SIZE];

    /* Clear both DP tables before starting */
    for (size_t i = 0; i < STATE_SIZE; ++i) {
        dp0[i] = 0;
        dp1[i] = 0;
    }

    seq_t *cur = dp0, *nxt = dp1;
    uint32_t *cur_act = act0, *nxt_act = act1;
    size_t cur_len = 0, nxt_len = 0;

    /* Initial state: empty board before first column */
    cur[0] = 1;
    cur_act[0] = 0;
    cur_len = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* Clear the entries of the next DP table that were used last time */
        for (size_t i = 0; i < nxt_len; ++i) {
            nxt[nxt_act[i]] = 0;
        }
        nxt_len = 0;

        /* Process all active states of the current frontier */
        for (size_t i = 0; i < cur_len; ++i) {
            uint32_t idx = cur_act[i];
            seq_t cnt = cur[idx];
            int m1 = idx >> STATE_BITS;          /* current column mask */
            int m2 = idx & FULL_MASK;           /* next column mask   */
            dfs_fill(m1, m2, 0, cnt, nxt, nxt_act, &nxt_len);
        }

        /* Reset current DP entries for reuse */
        for (size_t i = 0; i < cur_len; ++i) {
            cur[cur_act[i]] = 0;
        }

        /* Swap DP buffers and activity lists */
        seq_t *tmp_dp = cur;
        cur = nxt; nxt = tmp_dp;

        uint32_t *tmp_act = cur_act;
        cur_act = nxt_act; nxt_act = tmp_act;

        size_t tmp_len = cur_len;
        cur_len = nxt_len; nxt_len = tmp_len;
    }

    /* After processing n columns, the state (0,0) is the empty state */
    return cur[0];
}
