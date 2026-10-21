/*
 * A202418  Number of (n+2)X7 binary arrays with no more than one of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×7 binary arrays with no more than one of
 *            any consecutive three bits set in any row or column. The values
 *            are computed by dynamic programming over row‑patterns, not by a
 *            closed formula
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Precomputed data */
static size_t R = 0;             /* number of valid row patterns (≤7 bits)   */
static uint8_t *valid_masks = NULL;   /* masks of valid row patterns             */
static int *idx_by_mask = NULL;       /* index of a mask in valid_masks, or -1   */
static int *state_prev1 = NULL;       /* previous row for each state             */
static int *state_prev2 = NULL;       /* row before previous for each state      */
static size_t S = 0;                 /* number of states (valid (prev1,prev2))  */
static int16_t *pair_to_state = NULL;/* mapping (i,j) → state index (-1 if invalid) */
static int   *trans_offset = NULL;    /* offset of transitions for a state       */
static int   *trans_len    = NULL;    /* number of transitions for a state       */
static int   *trans_pool   = NULL;    /* concatenated list of next states        */
static size_t total_trans = 0;

/* DP buffers */
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;
static size_t current_rows = 2;       /* number of rows processed in dp_prev     */

/* Result cache */
static seq_t *a_vals = NULL;         /* a_vals[1..]                              */
static size_t a_capacity = 0;        /* allocated size of a_vals                 */
static bool precomputed = false;

/* ---------- Helper functions ---------------------------------------- */

static void init_combinatorics(void)
{
    /* Step 1: enumerate all 7‑bit masks and keep those satisfying the
       row‑constraint: no consecutive three bits may contain more than one 1. */
    uint8_t masks[128];
    idx_t count = 0;
    for (idx_t m = 0; m < 128; ++m) {
        bool ok = true;
        for (idx_t i = 0; i <= 4; ++i) {
            idx_t w = ((m >> i) & 7);                /* 3 bits window */
            if ((w & (w >> 1)) != 0) {              /* two 1s adjacent */
                ok = false; break;
            }
            if ((w & (w >> 2)) != 0) {              /* 1s with one bit between */
                ok = false; break;
            }
        }
        if (ok) {
            masks[count++] = (uint8_t)m;
        }
    }
    R = (size_t)count;
    valid_masks = (uint8_t*)malloc(R * sizeof(uint8_t));
    memcpy(valid_masks, masks, R * sizeof(uint8_t));

    /* index by mask */
    idx_by_mask = (int*)malloc(128 * sizeof(int));
    for (int i = 0; i < 128; ++i) idx_by_mask[i] = -1;
    for (int i = 0; i < (int)R; ++i) idx_by_mask[valid_masks[i]] = i;

    /* Build all valid state pairs (prev1, prev2) where the two rows are
       disjoint (no overlapping 1s). */
    S = 0;
    for (size_t i = 0; i < R; ++i) {
        for (size_t j = 0; j < R; ++j) {
            if ((valid_masks[i] & valid_masks[j]) == 0) ++S;
        }
    }
    state_prev1 = (int*)malloc(S * sizeof(int));
    state_prev2 = (int*)malloc(S * sizeof(int));
    pair_to_state = (int16_t*)malloc(R * R * sizeof(int16_t));
    for (size_t p = 0; p < R * R; ++p) pair_to_state[p] = -1;
    size_t st = 0;
    for (size_t i = 0; i < R; ++i) {
        for (size_t j = 0; j < R; ++j) {
            if ((valid_masks[i] & valid_masks[j]) == 0) {
                pair_to_state[i * R + j] = (int16_t)st;
                state_prev1[st] = (int)i;
                state_prev2[st] = (int)j;
                ++st;
            }
        }
    }

    /* Precompute transition lists: for each state s, list of successor states
       obtained by adding a compatible new row cur. */
    trans_len = (int*)malloc(S * sizeof(int));
    trans_offset = (int*)malloc(S * sizeof(int));
    total_trans = 0;
    for (size_t s = 0; s < S; ++s) {
        const int p1 = state_prev1[s];
        const int p2 = state_prev2[s];
        const uint8_t mask1 = valid_masks[p1];
        const uint8_t mask2 = valid_masks[p2];
        int cnt = 0;
        for (size_t k = 0; k < R; ++k) {
            if ((valid_masks[k] & (mask1 | mask2)) == 0) ++cnt;
        }
        trans_len[s] = cnt;
        total_trans += cnt;
    }
    trans_pool = (int*)malloc(total_trans * sizeof(int));
    /* Compute offsets */
    int offset = 0;
    for (size_t s = 0; s < S; ++s) {
        trans_offset[s] = offset;
        offset += trans_len[s];
    }
    /* Fill the pool */
    for (size_t s = 0; s < S; ++s) {
        const int p1 = state_prev1[s];
        const int p2 = state_prev2[s];
        const uint8_t mask1 = valid_masks[p1];
        const uint8_t mask2 = valid_masks[p2];
        int idx = trans_offset[s];
        for (size_t k = 0; k < R; ++k) {
            if ((valid_masks[k] & (mask1 | mask2)) == 0) {
                int next_state = (int)pair_to_state[k * R + p1];
                trans_pool[idx++] = next_state;
            }
        }
    }

    /* Allocate DP buffers */
    dp_prev = (seq_t*)malloc(S * sizeof(seq_t));
    dp_next = (seq_t*)malloc(S * sizeof(seq_t));
}

static void ensure_capacity(size_t needed)
{
    if (a_capacity >= needed + 1) return;
    size_t new_cap = a_capacity * 2;
    if (new_cap < needed + 1) new_cap = needed + 1;
    seq_t *new_a = (seq_t*)calloc(new_cap + 1, sizeof(seq_t));
    if (a_vals) {
        memcpy(new_a, a_vals, a_capacity * sizeof(seq_t));
        free(a_vals);
    }
    a_vals = new_a;
    a_capacity = new_cap;
}

/* ---------- Main function ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 */
    if (!precomputed) {
        init_combinatorics();
        /* Initialize DP for 2 rows: any pair of disjoint rows is a valid start. */
        for (size_t s = 0; s < S; ++s) dp_prev[s] = 1;
        for (size_t s = 0; s < S; ++s) dp_next[s] = 0;
        current_rows = 2; /* 2 rows have been processed */
        a_capacity = 16;
        a_vals = (seq_t*)calloc(a_capacity + 1, sizeof(seq_t));
        precomputed = true;
    }

    /* Extend computed range up to n (rows = n+2). */
    while (current_rows < (size_t)n + 2) {
        /* dp_prev → dp_next */
        for (size_t i = 0; i < S; ++i) dp_next[i] = 0;
        for (size_t s = 0; s < S; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            int offset = trans_offset[s];
            int len = trans_len[s];
            for (int k = 0; k < len; ++k) {
                int nxt = trans_pool[offset + k];
                dp_next[nxt] += val;
            }
        }
        /* swap buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
        ++current_rows;

        if (current_rows >= 3) {
            idx_t cur_n = (idx_t)(current_rows - 2);
            ensure_capacity(cur_n);
            seq_t sum = 0;
            for (size_t s = 0; s < S; ++s) sum += dp_prev[s];
            a_vals[cur_n] = sum;
        }
    }

    ensure_capacity((size_t)n);
    return a_vals[(size_t)n];
}
