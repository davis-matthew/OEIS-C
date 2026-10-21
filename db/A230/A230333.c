/*
 * A230333  Number of nX4 0..2 arrays x(i,j) with each element horizontally,
 *          vertically or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A230333
 *
 *  a(n) = number of n×4 arrays of values 0..2 with
 *  each element horizontally, vertically or antidiagonally
 *  adjacent to at least one element with value (x+1) mod 3,
 *  and with the upper‑left element equal to 0.
 *
 *  Computation: dynamic programming over rows.  Each row can be
 *  represented by 81 (3⁴) color patterns.  For a transition
 *  from row p to row q, we check that all pending requirements of
 *  row p are satisfied and compute which cells of row q still
 *  need a future neighbour.  The DP state consists of the
 *  current row pattern and a 4‑bit mask of pending cells.
 *  The number of rows n is processed in linear time with 81×16
 *  states.  A memoised cache allows arbitrary order of calls.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build system. */
extern seq_t _dummy;   /* silence unused warning */

/* Precomputed lookup tables */
static int init_tables_done = 0;
static uint8_t color_digits[81][4];   /* 3-digit base‑3 representation of a row pattern */
static uint8_t sat_mask[81][81];      /* for a transition, which cells of previous row are satisfied */
static uint8_t need_mask[81][81];     /* cells of current row that still need a neighbour */

/* DP arrays (state count = 81 × 16 = 1296) */
#define STATE_COUNT (81 * 16)
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;

/* Cache of the sequence values */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_len = 0;   /* number of cached terms */

/* Number of rows processed by the DP so far */
static idx_t dp_rows_done = 0;

/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    if (init_tables_done) return;
    init_tables_done = 1;

    /* Precompute color digits for each of the 81 patterns */
    for (int c = 0; c < 81; ++c) {
        int tmp = c;
        for (int j = 0; j < 4; ++j) {
            color_digits[c][j] = tmp % 3;
            tmp /= 3;
        }
    }

    /* Precompute sat_mask and need_mask for all transitions */
    for (int p = 0; p < 81; ++p) {
        for (int q = 0; q < 81; ++q) {
            uint8_t sat = 0;
            uint8_t need = 0;
            for (int j = 0; j < 4; ++j) {
                /* Does current cell (in row q) satisfy previous cell (in p)? */
                int req_prev = (color_digits[p][j] + 1) % 3;
                int sat_prev = 0;
                if (color_digits[q][j] == req_prev) sat_prev = 1;
                if (j > 0 && color_digits[q][j-1] == req_prev) sat_prev = 1;
                if (sat_prev) sat |= 1 << j;

                /* Does current cell have its own required neighbour? */
                int req_cur = (color_digits[q][j] + 1) % 3;
                int sat_cur = 0;
                if (j > 0 && color_digits[q][j-1] == req_cur) sat_cur = 1;
                if (j < 3 && color_digits[q][j+1] == req_cur) sat_cur = 1;
                if (color_digits[p][j] == req_cur) sat_cur = 1;
                if (j < 3 && color_digits[p][j+1] == req_cur) sat_cur = 1;
                if (!sat_cur) need |= 1 << j;
            }
            sat_mask[p][q]   = sat;
            need_mask[p][q] = need;
        }
    }

    /* Allocate DP storage */
    dp_cur  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    dp_next = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    if (!dp_cur || !dp_next) abort();
    memset(dp_cur, 0, STATE_COUNT * sizeof(seq_t));
    memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));
}

/* --------------------------------------------------------------------- */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 256;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort();
    a_cache = new_buf;
    a_cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
static void compute_up_to(idx_t target)
{
    if (target <= dp_rows_done) return;    /* already done */

    init_tables();    /* ensure tables ready */

    /* --- initialise for the first row if this is the first call --- */
    if (dp_rows_done == 0) {
        for (int c = 0; c < 81; ++c) {
            if (color_digits[c][0] != 0) continue;   /* upper‑left cell must be 0 */
            uint8_t need = 0;
            for (int j = 0; j < 4; ++j) {
                int req = (color_digits[c][j] + 1) % 3;
                int sat = 0;
                if (j > 0 && color_digits[c][j-1] == req) sat = 1;
                if (j < 3 && color_digits[c][j+1] == req) sat = 1;
                if (!sat) need |= 1 << j;
            }
            int id = (c << 4) | need;
            dp_cur[id] += 1;
        }
        dp_rows_done = 1;

        /* cache a(1) */
        ensure_cache_capacity(1);
        seq_t tot = 0;
        for (int id = 0; id < STATE_COUNT; ++id) {
            if ((id & 0xF) == 0) tot += dp_cur[id];
        }
        a_cache[0] = tot;
        a_cache_len = 1;
    }

    /* ----- process remaining rows ----- */
    for (idx_t r = dp_rows_done + 1; r <= target; ++r) {
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));

        for (int prev_id = 0; prev_id < STATE_COUNT; ++prev_id) {
            seq_t ways = dp_cur[prev_id];
            if (!ways) continue;
            int prev_col = prev_id >> 4;
            int prev_need = prev_id & 0xF;

            for (int cur_col = 0; cur_col < 81; ++cur_col) {
                uint8_t sat = sat_mask[prev_col][cur_col];
                if ((sat & prev_need) != prev_need) continue;   /* unmet requirement */

                uint8_t cur_need = need_mask[prev_col][cur_col];
                int cur_id = (cur_col << 4) | cur_need;
                dp_next[cur_id] += ways;
            }
        }

        /* swap DP buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        dp_rows_done = r;

        /* cache a(r) */
        ensure_cache_capacity(r);
        seq_t tot = 0;
        for (int id = 0; id < STATE_COUNT; ++id) {
            if ((id & 0xF) == 0) tot += dp_cur[id];
        }
        a_cache[r-1] = tot;
    }

    if (a_cache_len < target) a_cache_len = target;
}

/* --------------------------------------------------------------------- */
/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined behavior for n < 1 */
    compute_up_to(n);
    return a_cache[n-1];
}
