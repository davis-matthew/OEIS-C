/*
 * A230248  Number of nX5 0..3 arrays x(i,j) with each element horizontally or
 *          vertically next to at least one element with value (x(i,j)+1) mod
 *          4, and upper left element zero.
 * https://oeis.org/A230248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×5 arrays with elements 0..3 such that
 * each element has a horizontally or vertically adjacent element with
 * value (x+1) mod 4; the upper‑left element is 0.  This is computed by
 * dynamic programming over rows.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* --- constants -------------------------------------------------------- */
#define WIDTH       5          /* number of columns */
#define PAT_BITS    2          /* 2 bits per cell -> 4 values */
#define NUM_PAT     1024       /* 4^5 */
#define MASK_COUNT  32         /* 2^5 */

/* --- pattern data ----------------------------------------------------- */
static uint8_t pat_val[NUM_PAT][WIDTH];   /* each cell value */
static uint8_t hmask[NUM_PAT];            /* horizontal (x+1) mod 4 mask */

/* --- state data ------------------------------------------------------- */
static int16_t state_index[NUM_PAT][MASK_COUNT];
static uint16_t state_pat[NUM_PAT * MASK_COUNT];
static uint8_t  state_mask[NUM_PAT * MASK_COUNT];
static uint32_t state_cnt = 0;

/* --- transitions ------------------------------------------------------- */
struct TransVec {
    uint32_t *data;  /* indices of destination states */
    uint32_t  cnt;   /* number of transitions */
};
static struct TransVec *trans = NULL;

/* --- DP buffers -------------------------------------------------------- */
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;
static size_t dp_size = 0;

/* --- result cache ----------------------------------------------------- */
static seq_t *res_arr   = NULL;
static idx_t  res_len   = 0;
static idx_t  max_computed = 0;

/* --------------------------------------------------------------------- */
/* Helper: fill pat_val and hmask for all patterns */
static void init_patterns(void)
{
    for (int id = 0; id < NUM_PAT; ++id) {
        int x = id;
        for (int j = 0; j < WIDTH; ++j) {
            pat_val[id][j] = x & 3;
            x >>= 2;
        }
        uint8_t h = 0;
        for (int j = 0; j < WIDTH; ++j) {
            uint8_t v = pat_val[id][j];
            if (j > 0 && pat_val[id][j-1] == (v + 1) % 4) h |= 1u << j;
            if (j + 1 < WIDTH && pat_val[id][j+1] == (v + 1) % 4) h |= 1u << j;
        }
        hmask[id] = h;
    }
}

/* --------------------------------------------------------------------- */
/* Build state table: for each pattern and each admissible mask. */
static void build_states(void)
{
    memset(state_index, -1, sizeof(state_index));
    state_cnt = 0;
    for (int p = 0; p < NUM_PAT; ++p) {
        /* positions that must be satisfied from below (no horizontal neighbor) */
        int unsat_pos = (~hmask[p]) & 0x1F;
        /* iterate over all subsets of unsat_pos */
        int sub = unsat_pos;
        while (1) {
            int mask = sub;
            state_index[p][mask] = (int16_t)state_cnt;
            state_pat[state_cnt] = (uint16_t)p;
            state_mask[state_cnt] = (uint8_t)mask;
            ++state_cnt;
            if (sub == 0) break;
            sub = (sub - 1) & unsat_pos;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Build transition lists between states. */
static void build_transitions(void)
{
    trans = (struct TransVec *)calloc(state_cnt, sizeof(struct TransVec));
    for (uint32_t s = 0; s < state_cnt; ++s) {
        uint16_t p = state_pat[s];
        uint8_t   m = state_mask[s];
        for (int q = 0; q < NUM_PAT; ++q) {
            /* q must satisfy all required columns from above. */
            int ok = 1;
            for (int j = 0; j < WIDTH; ++j) {
                if (m & (1u << j)) {
                    if (pat_val[q][j] != (pat_val[p][j] + 1) % 4) {
                        ok = 0;
                        break;
                    }
                }
            }
            if (!ok) continue;
            /* vertical satisfaction from row p to q */
            uint8_t vertical = 0;
            for (int j = 0; j < WIDTH; ++j) {
                if (pat_val[p][j] == (pat_val[q][j] + 1) % 4)
                    vertical |= 1u << j;
            }
            /* unsatisfied cells in row q after both horizontal and vertical checks */
            uint8_t unsat_q = ((~hmask[q]) & (~vertical)) & 0x1F;
            int dest = state_index[q][unsat_q];
            if (dest < 0) continue;            /* safety */
            /* append dest to transition list */
            uint32_t *d = trans[s].data;
            uint32_t newcnt = trans[s].cnt + 1;
            d = (uint32_t *)realloc(d, newcnt * sizeof(uint32_t));
            d[newcnt - 1] = (uint32_t)dest;
            trans[s].data = d;
            trans[s].cnt = newcnt;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Ensure DP buffers have capacity for all states. */
static void ensure_dp(size_t sz)
{
    if (dp_size < sz) {
        free(dp_prev);
        free(dp_next);
        dp_prev = (seq_t *)calloc(sz, sizeof(seq_t));
        dp_next = (seq_t *)calloc(sz, sizeof(seq_t));
        dp_size = sz;
    }
}

/* --------------------------------------------------------------------- */
/* Ensure result array can hold index n. */
static void ensure_res(idx_t n)
{
    if (n >= res_len) {
        size_t newlen = n + 1;
        seq_t *newarr = (seq_t *)realloc(res_arr, newlen * sizeof(seq_t));
        for (idx_t i = res_len; i < newlen; ++i)
            newarr[i] = 0;
        res_arr = newarr;
        res_len = newlen;
    }
}

/* --------------------------------------------------------------------- */
/* Main function demanded by the task. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_patterns();
        build_states();
        build_transitions();
        ensure_dp(state_cnt);
        ensure_res(n);

        /* initial DP for first row (row index 1) */
        memset(dp_prev, 0, state_cnt * sizeof(seq_t));
        for (int p = 0; p < NUM_PAT; ++p) {
            if (pat_val[p][0] != 0) continue;              /* upper‑left must be 0 */
            uint8_t unsat = (~hmask[p]) & 0x1F;
            int idx = state_index[p][unsat];
            if (idx < 0) continue;
            dp_prev[idx] += (seq_t)1;
        }

        /* compute a(1) */
        seq_t a1 = 0;
        for (uint32_t s = 0; s < state_cnt; ++s)
            if (state_mask[s] == 0)
                a1 += dp_prev[s];
        res_arr[1] = a1;
        max_computed = 1;
        initialized = 1;
    }

    if (n <= max_computed)
        return res_arr[n];

    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        memset(dp_next, 0, state_cnt * sizeof(seq_t));
        for (uint32_t s = 0; s < state_cnt; ++s) {
            if (dp_prev[s] == 0) continue;
            seq_t cnt = dp_prev[s];
            uint32_t tc = trans[s].cnt;
            if (tc == 0) continue;
            for (uint32_t k = 0; k < tc; ++k) {
                uint32_t dst = trans[s].data[k];
                dp_next[dst] += cnt;
            }
        }
        /* swap current and next DP buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;

        /* compute a(cur) */
        seq_t ax = 0;
        for (uint32_t s = 0; s < state_cnt; ++s)
            if (state_mask[s] == 0)
                ax += dp_prev[s];

        ensure_res(cur);
        res_arr[cur] = ax;
    }

    max_computed = n;
    return res_arr[n];
}
