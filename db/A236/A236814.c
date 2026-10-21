/*
 * A236814  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over row pairs with a common constant c = max + lower‑median – min. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define ROWS   81          /* 3^4 possible rows (0..2)^4 */
#define MAXC   5           /* constants {0,1,2,3,4} (4 actually used) */
#define MAXEDGES (ROWS * ROWS)   /* maximum edges per constant */

/* -------------  preprocessing  ------------ */
static bool precomputed = false;

static int digit[ROWS][4];
static int edge_count[MAXC][ROWS];
static int edge_offset[MAXC][ROWS + 1];
static int edge_to[MAXC][MAXEDGES];
static seq_t dp_cur[MAXC][ROWS];
static seq_t dp_nxt[MAXC][ROWS];

static seq_t *totals = NULL;
static size_t totals_size = 0;
static idx_t max_computed = 0;

/* helper: compute constant for pair of rows (0..80) */
static int pair_const(int r1, int r2)
{
    int a, b, c, d, minv, maxv, med;
    int vals[4];
    int constant = -1;

    for (int j = 0; j < 3; ++j) {
        a = digit[r1][j];
        b = digit[r1][j + 1];
        c = digit[r2][j];
        d = digit[r2][j + 1];
        vals[0] = a; vals[1] = b; vals[2] = c; vals[3] = d;

        /* simple insertion sort */
        for (int i = 0; i < 3; ++i)
            for (int k = i + 1; k < 4; ++k)
                if (vals[i] > vals[k]) {
                    int t = vals[i]; vals[i] = vals[k]; vals[k] = t;
                }

        minv = vals[0];
        med  = vals[1];          /* lower median of four values */
        maxv = vals[3];
        int cur = maxv + med - minv;

        if (constant == -1)
            constant = cur;
        else if (constant != cur)
            return -1;
    }
    return constant;
}

static void precompute(void)
{
    /* digits of each row */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int k = 0; k < 4; ++k) {
            digit[r][k] = x % 3;
            x /= 3;
        }
    }

    /* count edges per constant */
    for (int c = 0; c < MAXC; ++c)
        for (int r = 0; r < ROWS; ++r)
            edge_count[c][r] = 0;

    for (int u = 0; u < ROWS; ++u) {
        for (int v = 0; v < ROWS; ++v) {
            int c = pair_const(u, v);
            if (c >= 0)
                edge_count[c][u]++;
        }
    }

    /* offsets */
    for (int c = 0; c < MAXC; ++c) {
        edge_offset[c][0] = 0;
        int sum = 0;
        for (int r = 0; r < ROWS; ++r) {
            sum += edge_count[c][r];
            edge_offset[c][r + 1] = sum;
        }
    }

    /* temporary counters for building adjacency list */
    static int pos[MAXC][ROWS];
    for (int c = 0; c < MAXC; ++c)
        for (int r = 0; r < ROWS; ++r)
            pos[c][r] = 0;

    for (int u = 0; u < ROWS; ++u) {
        for (int v = 0; v < ROWS; ++v) {
            int c = pair_const(u, v);
            if (c >= 0) {
                int idx = edge_offset[c][u] + pos[c][u];
                edge_to[c][idx] = v;
                pos[c][u]++;
            }
        }
    }

    /* initialize DP for 0 steps (one row) */
    for (int c = 0; c < MAXC; ++c)
        for (int r = 0; r < ROWS; ++r)
            dp_cur[c][r] = 1;

    /* totals array: totals[0] unused, totals[1] will be computed */
    totals_size = 1;
    totals = (seq_t *)malloc(totals_size * sizeof(seq_t));
    totals[0] = 0;

    precomputed = true;
}

/* -------------  sequence function  ------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset 1 */

    if (!precomputed)
        precompute();

    if (n <= max_computed)
        return totals[n];

    /* extend DP up to n */
    for (idx_t step = max_computed + 1; step <= n; ++step) {
        /* zero dp_nxt */
        for (int c = 0; c < MAXC; ++c)
            for (int r = 0; r < ROWS; ++r)
                dp_nxt[c][r] = 0;

        /* transitions */
        for (int c = 0; c < MAXC; ++c) {
            for (idx_t u = 0; u < ROWS; ++u) {
                seq_t val = dp_cur[c][u];
                if (val == 0) continue;
                int offs = edge_offset[c][u];
                int cnt  = edge_count[c][u];
                for (int e = 0; e < cnt; ++e) {
                    idx_t v = edge_to[c][offs + e];
                    dp_nxt[c][v] += val;
                }
            }
        }

        /* compute total for this step (arrays of height step+1) */
        seq_t total = 0;
        for (int c = 0; c < MAXC; ++c)
            for (int r = 0; r < ROWS; ++r)
                total += dp_nxt[c][r];

        /* resize totals array if needed */
        if (totals_size <= step) {
            size_t new_size = step + 1;
            seq_t *new_ptr = (seq_t *)realloc(totals, new_size * sizeof(seq_t));
            for (size_t i = totals_size; i < new_size; ++i)
                new_ptr[i] = 0;
            totals = new_ptr;
            totals_size = new_size;
        }
        totals[step] = total;

        /* swap dp_cur and dp_nxt by copying */
        for (int c = 0; c < MAXC; ++c)
            for (int r = 0; r < ROWS; ++r)
                dp_cur[c][r] = dp_nxt[c][r];
    }

    max_computed = n;
    return totals[n];
}
