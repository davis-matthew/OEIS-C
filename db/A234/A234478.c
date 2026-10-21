/*
 * A234478  Number of (n+1) X (3+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,…,7} in which each 2×2
 *            subblock has diagonal sum differing from its antidiagonal sum by
 *            exactly 1
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static int a234478_initialized = 0;
static size_t a234478_K = 0;
static seq_t *a234478_W = NULL;
static seq_t *a234478_cur = NULL;
static seq_t *a234478_next = NULL;
static int a234478_idx_map[15][15][15];
static int a234478_d1[3375];
static int a234478_d2[3375];
static int a234478_d3[3375];
static unsigned char a234478_predcnt[3375];
static int a234478_pred[3375][8];

static void a234478_init(void)
{
    int i, j, k, idx = 0;
    /* build the map from (d1,d2,d3) to an index */
    for (i = 0; i < 15; ++i)
        for (j = 0; j < 15; ++j)
            for (k = 0; k < 15; ++k)
                a234478_idx_map[i][j][k] = -1;

    /* enumerate all admissible difference vectors */
    for (i = -7; i <= 7; ++i)
        for (j = -7; j <= 7; ++j)
            for (k = -7; k <= 7; ++k) {
                int d1 = i, d2 = j, d3 = k;
                int low = 0;
                if (d1 > low)          low = d1;
                if (d1 + d2 > low)     low = d1 + d2;
                if (d1 + d2 + d3 > low) low = d1 + d2 + d3;
                int up = 7;
                if (d1 + 7 < up)           up = d1 + 7;
                if (d1 + d2 + 7 < up)     up = d1 + d2 + 7;
                if (d1 + d2 + d3 + 7 < up) up = d1 + d2 + d3 + 7;
                int w = up - low + 1;
                if (w > 0) {
                    a234478_idx_map[d1 + 7][d2 + 7][d3 + 7] = idx;
                    a234478_d1[idx] = d1;
                    a234478_d2[idx] = d2;
                    a234478_d3[idx] = d3;
                    ++idx;
                }
            }
    a234478_K = idx;

    /* allocate tables */
    a234478_W   = (seq_t*)malloc(a234478_K * sizeof(seq_t));
    a234478_cur = (seq_t*)malloc(a234478_K * sizeof(seq_t));
    a234478_next = (seq_t*)malloc(a234478_K * sizeof(seq_t));

    /* compute weight of each difference vector */
    for (i = 0; i < (int)a234478_K; ++i) {
        int d1 = a234478_d1[i];
        int d2 = a234478_d2[i];
        int d3 = a234478_d3[i];
        int low = 0;
        if (d1 > low)          low = d1;
        if (d1 + d2 > low)     low = d1 + d2;
        if (d1 + d2 + d3 > low) low = d1 + d2 + d3;
        int up = 7;
        if (d1 + 7 < up)           up = d1 + 7;
        if (d1 + d2 + 7 < up)     up = d1 + d2 + 7;
        if (d1 + d2 + d3 + 7 < up) up = d1 + d2 + d3 + 7;
        int w = up - low + 1;
        if (w < 0) w = 0;
        a234478_W[i] = (seq_t)w;
    }

    /* precompute predecessors for each state */
    for (i = 0; i < (int)a234478_K; ++i) {
        a234478_predcnt[i] = 0;
        int d1 = a234478_d1[i];
        int d2 = a234478_d2[i];
        int d3 = a234478_d3[i];
        for (int s = 0; s < 8; ++s) {
            int nd1 = d1 + ((s & 1) ? 1 : -1);
            int nd2 = d2 + ((s & 2) ? 1 : -1);
            int nd3 = d3 + ((s & 4) ? 1 : -1);
            if (nd1 < -7 || nd1 > 7 || nd2 < -7 || nd2 > 7 || nd3 < -7 || nd3 > 7)
                continue;
            int prev = a234478_idx_map[nd1 + 7][nd2 + 7][nd3 + 7];
            if (prev >= 0)
                a234478_pred[i][a234478_predcnt[i]++] = prev;
        }
    }
    a234478_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!a234478_initialized) a234478_init();

    const size_t K = a234478_K;
    seq_t *cur  = a234478_cur;
    seq_t *next = a234478_next;

    /* initial state: one row */
    for (size_t i = 0; i < K; ++i)
        cur[i] = a234478_W[i];

    /* perform n transitions (add n additional rows) */
    for (idx_t step = 0; step < n; ++step) {
        for (size_t i = 0; i < K; ++i) {
            seq_t s = 0;
            unsigned char pc = a234478_predcnt[i];
            int *pred = a234478_pred[i];
            for (unsigned char p = 0; p < pc; ++p)
                s += cur[pred[p]];
            next[i] = s * a234478_W[i];
        }
        /* swap buffers */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum over all ending states */
    seq_t total = 0;
    for (size_t i = 0; i < K; ++i)
        total += cur[i];
    return total;
}
