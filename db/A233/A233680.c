/*
 * A233680  Number of (n+1) X (6+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 6.
 * https://oeis.org/A233680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 arrays with entries 0..3 such that *
 *            every 2×2 subblock has the sum of the squares of the edge
 *            differences * equal to 6. The sequence is computed by dynamic
 *            programming over all * possible rows (4⁷ = 16384 states). Term
 *            a(1) = 32456, a(2) = 621904, * … as given in the OEI...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Constants for the state graph */
#define NUM_STATES 16384   /* 4⁷ possible rows */
#define MAX_COLS   7       /* 7 columns per row */

/* Decode a state index into an array of 7 values (0..3). */
static uint8_t row_vals[NUM_STATES][MAX_COLS];

/* For each adjacent pair (a,b) of the previous row and each left value
 * of the current row, a bitmask of allowed values for the next current
 * cell is stored in allow_next[a][b][prev]. */
static uint8_t allow_next[4][4][4];

/* Succession information: for each state r we store the starting index
 * of its successors in succ_flat and the number of successors. */
static size_t succ_start[NUM_STATES];
static size_t succ_len[NUM_STATES];
static uint16_t *succ_flat = NULL;

/* DP arrays */
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;

/* Cache of computed sequence values */
static seq_t *result_cache = NULL;
static size_t cache_size   = 0;
static size_t max_n_done   = 0;

/* Initialisation flag */
static bool initialized = false;

/* ------------------------------------------------------------------- */
/* Helper to compute all successors for a given previous row. */
static void rec_build_s_bit(size_t r_idx, int pos, uint16_t sbits,
                            uint16_t **arr, size_t *len, size_t *cap)
{
    if (pos == MAX_COLS) {
        if (*len == *cap) {
            size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
            uint16_t *tmp = (uint16_t *)realloc(*arr, newcap * sizeof(uint16_t));
            if (!tmp) abort();
            *arr  = tmp;
            *cap  = newcap;
        }
        (*arr)[(*len)++] = sbits;
        return;
    }
    if (pos == 0) {
        for (uint8_t cur = 0; cur < 4; ++cur)
            rec_build_s_bit(r_idx, 1,
                    (uint16_t)cur, arr, len, cap);
    } else {
        uint8_t a   = row_vals[r_idx][pos-1];
        uint8_t b   = row_vals[r_idx][pos];
        uint8_t prev = (sbits >> ((pos-1)*2)) & 3;
        uint8_t mask = allow_next[a][b][prev];
        while (mask) {
            uint8_t cur = __builtin_ctz(mask);
            mask &= mask - 1;
            rec_build_s_bit(r_idx, pos+1,
                    sbits | ((uint16_t)cur << (pos*2)),
                    arr, len, cap);
        }
    }
}

/* ------------------------------------------------------------------- */
/* Build the entire graph of allowed transitions between rows. */
static void build_adjacency(void)
{
    /* Decode each state into its 7 digits. */
    for (size_t idx = 0; idx < NUM_STATES; ++idx) {
        for (int col = 0; col < MAX_COLS; ++col)
            row_vals[idx][col] = (uint8_t)((idx >> (col*2)) & 3);
    }

    /* Pre‑compute masks of allowed next values for each (a,b,prev). */
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int prev = 0; prev < 4; ++prev) {
                uint8_t mask = 0;
                for (int cur = 0; cur < 4; ++cur) {
                    int h_top    = abs((int)a - (int)b);
                    int h_bottom = abs((int)prev - (int)cur);
                    int v_left   = abs((int)a - (int)prev);
                    int v_right  = abs((int)b - (int)cur);
                    int sumsq = h_top*h_top + h_bottom*h_bottom
                              + v_left*v_left + v_right*v_right;
                    if (sumsq == 6) mask |= (1 << cur);
                }
                allow_next[a][b][prev] = mask;
            }

    /* Build successor lists for all rows. */
    size_t total_edges = 0;
    static uint16_t *tmp_arr[NUM_STATES];
    static size_t   tmp_len[NUM_STATES];
    static size_t   tmp_cap[NUM_STATES];

    for (size_t r = 0; r < NUM_STATES; ++r) {
        tmp_arr[r] = NULL;
        tmp_len[r] = 0;
        tmp_cap[r] = 0;
        rec_build_s_bit(r, 0, 0, &tmp_arr[r], &tmp_len[r], &tmp_cap[r]);

        succ_len[r]   = tmp_len[r];
        succ_start[r] = total_edges;
        total_edges  += tmp_len[r];
    }

    succ_flat = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    if (!succ_flat) abort();

    size_t pos = 0;
    for (size_t r = 0; r < NUM_STATES; ++r) {
        memcpy(succ_flat + pos, tmp_arr[r], succ_len[r] * sizeof(uint16_t));
        pos += succ_len[r];
        free(tmp_arr[r]);
    }
}

/* ------------------------------------------------------------------- */
/* One‑time initialisation of all static data. */
static void init_once(void)
{
    build_adjacency();

    dp_prev = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
    dp_next = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
    if (!dp_prev || !dp_next) abort();

    /* Zero the DP vectors. */
    memset(dp_prev, 0, NUM_STATES * sizeof(seq_t));
    memset(dp_next, 0, NUM_STATES * sizeof(seq_t));

    /* First row: all rows are allowed. */
    for (size_t i = 0; i < NUM_STATES; ++i)
        dp_prev[i] = (seq_t)1;

    /* Initialise the answer cache. */
    result_cache = (seq_t *)malloc(sizeof(seq_t));
    if (!result_cache) abort();
    result_cache[0] = (seq_t)0;  /* unused */
    cache_size   = 1;
    max_n_done   = 0;
    initialized = true;
}

/* ------------------------------------------------------------------- */
/* Public function returning a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!initialized) init_once();

    size_t target = (size_t)n;   /* n is guaranteed ≥1 */

    if (target >= cache_size) {
        /* Grow the cache to hold values up to target. */
        seq_t *tmp = (seq_t *)realloc(result_cache,
                                      (target + 1) * sizeof(seq_t));
        if (!tmp) abort();
        result_cache = tmp;
        cache_size = target + 1;

        for (size_t step = max_n_done + 1; step <= target; ++step) {
            /* reset dp_next */
            memset(dp_next, 0, NUM_STATES * sizeof(seq_t));

            /* perform one DP transition */
            for (size_t r = 0; r < NUM_STATES; ++r) {
                seq_t val = dp_prev[r];
                if (val == (seq_t)0) continue;
                size_t st = succ_start[r];
                size_t len = succ_len[r];
                for (size_t k = 0; k < len; ++k) {
                    size_t s = succ_flat[st + k];
                    dp_next[s] += val;
                }
            }

            /* swap the vectors */
            seq_t *tmpv = dp_prev;
            dp_prev = dp_next;
            dp_next = tmpv;

            /* sum all states to get a(step) */
            seq_t sum = (seq_t)0;
            for (size_t i = 0; i < NUM_STATES; ++i)
                sum += dp_prev[i];
            result_cache[step] = sum;
        }
        max_n_done = target;
    }
    return result_cache[n];
}
