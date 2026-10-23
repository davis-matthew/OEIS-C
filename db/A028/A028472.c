/*
 * A028472  Number of perfect matchings in graph P_{10} X P_{n}.
 * https://oeis.org/A028472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of perfect matchings in graph P_{10} × P_{n} The
 *            sequence is computed by a transfer‑matrix algorithm for domino
 *            tilings of a 10 × n board. For each column we propagate a DP
 *            over 2^10 = 1024 possible occupancy masks. The transition counts
 *            are pre‑computed once by a depth‑first search...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 10
#define STATES (1 << WIDTH)

/* Helper for transition generation */
static void dfs_gen(int pos, int curMask, int nextMask,
                    uint16_t *cnt_arr)
{
    if (pos == WIDTH) {          /* column completely filled */
        cnt_arr[nextMask]++;     /* one way to reach nextMask */
        return;
    }
    if (curMask & (1 << pos)) { /* cell already occupied */
        dfs_gen(pos + 1, curMask, nextMask, cnt_arr);
        return;
    }
    /* place vertical domino covering pos and pos+1 */
    if (pos + 1 < WIDTH && !(curMask & (1 << (pos + 1)))) {
        int newMask = curMask | (1 << pos) | (1 << (pos + 1));
        dfs_gen(pos + 1, newMask, nextMask, cnt_arr);
    }
    /* place horizontal domino extending to the next column */
    {
        int newMask = curMask | (1 << pos);
        int newNext = nextMask | (1 << pos);
        dfs_gen(pos + 1, newMask, newNext, cnt_arr);
    }
}

/* Transition counts table: trans_counts[mask][nextMask] = number of ways */
static uint16_t trans_counts[STATES][STATES] = {0};
static bool trans_initialized = false;

/* Initialise transition table once */
static void init_transitions(void)
{
    if (trans_initialized) return;
    for (int mask = 0; mask < STATES; ++mask) {
        uint16_t cnt_arr[STATES] = {0};
        dfs_gen(0, mask, 0, cnt_arr);
        for (int nt = 0; nt < STATES; ++nt) {
            trans_counts[mask][nt] = cnt_arr[nt];
        }
    }
    trans_initialized = true;
}

/* Global DP state and memoisation */
static seq_t cur[STATES] = {0};
static seq_t next[STATES] = {0};
static idx_t cur_col = 0;          /* number of columns processed so far */
static seq_t *memo = NULL;         /* memoised answers: memo[n] = a(n) */
static idx_t memo_len = 0;         /* next free index in memo */
static idx_t memo_cap = 0;         /* capacity of memo array */

/* Ensure memo array large enough */
static void ensure_memo_capacity(idx_t needed)
{
    if (needed < memo_cap) return;
    idx_t new_cap = memo_cap ? memo_cap * 2 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) return;  /* allocation failure – terminate silently */
    memo = new_mem;
    memo_cap = new_cap;
}

/* Main function exported */
seq_t a(idx_t n)
{
    init_transitions();

    /* Initialise DP state if first call */
    if (cur_col == 0) {
        memset(cur, 0, sizeof(cur));
        cur[0] = 1;
        /* initialise memo */
        memo = NULL;
        memo_cap = 0;
        memo_len = 0;
        ensure_memo_capacity(0);
        memo[0] = 1;
        memo_len = 1;
    }

    /* If n already computed, return */
    if (n < memo_len) return memo[n];

    /* Extend memo and compute DP up to n */
    for (idx_t col = cur_col + 1; col <= n; ++col) {
        memset(next, 0, sizeof(next));

        for (int m = 0; m < STATES; ++m) {
            seq_t val = cur[m];
            if (val == 0) continue;
            for (int t = 0; t < STATES; ++t) {
                uint16_t cnt = trans_counts[m][t];
                if (cnt == 0) continue;
                next[t] += val * (seq_t)cnt;
            }
        }
        memcpy(cur, next, sizeof(cur));

        /* Memoise result for this n */
        ensure_memo_capacity(col);
        memo[col] = cur[0];
    }

    cur_col = n;
    memo_len = n + 1;
    return memo[n];
}
