/*
 * A237096  Number of (n+1) X (5+1) 0..2 arrays with the maximum plus the
 *          lower median minus the upper median of every 2 X 2 subblock equal.
 * https://oeis.org/A237096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A237096: Number of (n+1)×6 arrays with entries 0..2 such that the
 *  function max + lower_median − upper_median is the same for every 2×2
 *  subblock.  The algorithm builds, once, all compatibility lists for
 *  the 729 possible rows and then runs a dynamic‑programming walk counts
 *  for each of the three possible constant values.  The result is the
 *  sum of the three counts.  The implementation works for any seq_t
 *  and idx_t supplied by the build system.  It runs in O(n·729) time
 *  and uses < 32 kB of memory. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----------   Tables and state   ------------------------------------- */

static uint8_t val_lookup[81];                /* table of 2×2 block values */
static uint8_t row_digits[729][6];            /* digits of each row */

/* adjacency lists: adj[C][i] → neighbors of row i (C = 0,1,2) */
static uint16_t **adj[3];
static size_t *adj_sz[3];
static size_t *adj_cap[3];

static int init_done = 0;

/* ----------   Helper: block value   --------------------------------- */

static inline uint8_t block_val(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return val_lookup[a * 27 + b * 9 + c * 3 + d];
}

/* ----------   Initialization   ------------------------------------- */
static void init_data(void)
{
    if (init_done) return;
    init_done = 1;

    /* (1) Build block value lookup table */
    for (int t = 0; t < 81; ++t) {
        uint8_t v[4];
        int x = t;
        for (int k = 0; k < 4; ++k) { v[k] = x % 3; x /= 3; }

        /* sort v[0..3] */
        for (int i = 0; i < 4; ++i)
            for (int j = i + 1; j < 4; ++j)
                if (v[i] > v[j]) { uint8_t u = v[i]; v[i] = v[j]; v[j] = u; }

        uint8_t maxv  = v[3];
        uint8_t lo_med = v[1];
        uint8_t up_med = v[2];
        val_lookup[t] = maxv + lo_med - up_med;
    }

    /* (2) Decode all 729 rows into base‑3 digits */
    for (int r = 0; r < 729; ++r) {
        int x = r;
        for (int k = 0; k < 6; ++k) { row_digits[r][k] = x % 3; x /= 3; }
    }

    /* (3) Allocate adjacency structures */
    for (int C = 0; C < 3; ++C) {
        adj[C]    = (uint16_t **)malloc(729 * sizeof(uint16_t *));
        adj_sz[C] = (size_t *)malloc(729 * sizeof(size_t));
        adj_cap[C] = (size_t *)malloc(729 * sizeof(size_t));
        for (int i = 0; i < 729; ++i) {
            adj[C][i] = NULL;
            adj_sz[C][i] = 0;
            adj_cap[C][i] = 0;
        }
    }

    /* (4) Build adjacency lists by examining all row pairs */
    for (int i = 0; i < 729; ++i) {
        for (int j = i; j < 729; ++j) {
            uint8_t first_val = 0, v;
            int same = 1;
            for (int col = 0; col < 5; ++col) {
                uint8_t a = row_digits[i][col];
                uint8_t b = row_digits[i][col + 1];
                uint8_t c = row_digits[j][col];
                uint8_t d = row_digits[j][col + 1];
                v = block_val(a, b, c, d);
                if (col == 0) first_val = v;
                else if (v != first_val) { same = 0; break; }
            }
            if (!same) continue;
            uint8_t C = first_val;   /* 0, 1 or 2 */

            /* add j as neighbor of i */
            if (adj_sz[C][i] == adj_cap[C][i]) {
                size_t newcap = adj_cap[C][i] == 0 ? 4 : adj_cap[C][i] * 2;
                uint16_t *tmp = (uint16_t *)realloc(adj[C][i], newcap * sizeof(uint16_t));
                adj[C][i] = tmp;
                adj_cap[C][i] = newcap;
            }
            adj[C][i][adj_sz[C][i]++] = (uint16_t)j;

            /* add reverse edge if distinct */
            if (i != j) {
                if (adj_sz[C][j] == adj_cap[C][j]) {
                    size_t newcap = adj_cap[C][j] == 0 ? 4 : adj_cap[C][j] * 2;
                    uint16_t *tmp = (uint16_t *)realloc(adj[C][j], newcap * sizeof(uint16_t));
                    adj[C][j] = tmp;
                    adj_cap[C][j] = newcap;
                }
                adj[C][j][adj_sz[C][j]++] = (uint16_t)i;
            }
        }
    }
}

/* ----------   DP for a fixed constant C   --------------------------- */
static seq_t count_for_C(uint8_t C, idx_t n)
{
    static seq_t *dp[2] = {NULL, NULL};
    static int allocated = 0;
    if (!allocated) {
        dp[0] = (seq_t *)malloc(729 * sizeof(seq_t));
        dp[1] = (seq_t *)malloc(729 * sizeof(seq_t));
        allocated = 1;
    }

    seq_t *cur = dp[0];
    seq_t *nxt = dp[1];

    /* first row can be any of the 729 rows */
    for (int i = 0; i < 729; ++i) cur[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 729; ++i) nxt[i] = 0;
        for (int i = 0; i < 729; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            uint16_t *nbr = adj[C][i];
            size_t sz = adj_sz[C][i];
            for (size_t k = 0; k < sz; ++k)
                nxt[nbr[k]] += val;
        }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t total = 0;
    for (int i = 0; i < 729; ++i) total += cur[i];
    return total;
}

/* ----------   Main sequence function   ----------------------------- */
seq_t a(idx_t n)
{
    init_data();

    if (n == 0) return 0;          /* below the offset – undefined */

    seq_t result = 0;
    result += count_for_C(0, n);
    result += count_for_C(1, n);
    result += count_for_C(2, n);
    return result;
}
