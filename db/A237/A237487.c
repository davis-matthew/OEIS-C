/*
 * A237487  Number of (n+1)X(6+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A237487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: a(n) = number of (n+1)x7 arrays with entries 0..3 such that
   the value max + min – lower median of every 2x2 subblock is the same
   (for all subblocks).  This equals the total number of walks of length n
   in the graph whose vertices are the 7‑cell rows (0–3) and that respect
   the above local constraint, summed over the 4 possible constant values. */

#include <stdint.h>
#include <stdlib.h>

#define MAX_STATE (1 << 14)          /* 7 columns, 2 bits each */

/* adjacency structure for each state */
typedef struct {
    uint16_t *data;   /* array of next states */
    uint16_t len;     /* current length */
    uint16_t cap;     /* allocated capacity */
} StateAdj;

/* adjacency lists: [K][state] */
static StateAdj adj[4][MAX_STATE];

/* 256‑byte table: F_arr[a,b,c,d] = a + d – median_lower */
static uint8_t F_arr[256];
static int adjacency_built = 0;

/* ------------------------------------------------------------------------- */
/* Pre‑compute F_arr table */
static void init_F_arr(void)
{
    int arr[4];
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                for (int d = 0; d < 4; ++d) {
                    arr[0] = a; arr[1] = b; arr[2] = c; arr[3] = d;
                    /* simple sorting */
                    for (int i = 0; i < 3; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (arr[i] > arr[j]) {
                                int t = arr[i];
                                arr[i] = arr[j];
                                arr[j] = t;
                            }
                    int f = arr[0] + arr[3] - arr[1];
                    F_arr[(a << 6) | (b << 4) | (c << 2) | d] = (uint8_t)f;
                }
}

/* ------------------------------------------------------------------------- */
/* Add an edge to the adjacency list of state 'from' for given K */
static void add_adj_edge(int K, uint16_t from, uint16_t to)
{
    StateAdj *st = &adj[K][from];
    if (st->cap == 0) {
        st->cap = 4;
        st->data = (uint16_t *)malloc(st->cap * sizeof(uint16_t));
    } else if (st->len == st->cap) {
        st->cap <<= 1;
        st->data = (uint16_t *)realloc(st->data, st->cap * sizeof(uint16_t));
    }
    st->data[st->len++] = to;
}

/* ------------------------------------------------------------------------- */
/* Recursive generation of all next rows for a given state and K */
static void dfs_gen(int K, const uint8_t *s_cells,
                    uint8_t *t_cells, int idx, uint16_t s_state)
{
    if (idx == 6) {   /* completed row of length 7 */
        uint16_t tcode = 0;
        for (int i = 0; i < 7; ++i)
            tcode |= ((uint16_t)t_cells[i]) << (2 * i);
        add_adj_edge(K, s_state, tcode);
        return;
    }

    int a = s_cells[idx];
    int b = s_cells[idx + 1];
    int c = t_cells[idx];
    int base = (a << 6) | (b << 4) | (c << 2);
    for (int cand = 0; cand < 4; ++cand)
        if (F_arr[base | cand] == K) {
            t_cells[idx + 1] = (uint8_t)cand;
            dfs_gen(K, s_cells, t_cells, idx + 1, s_state);
        }
}

/* Generate all adjacency lists */
static void init_adjacency(void)
{
    if (adjacency_built)
        return;
    adjacency_built = 1;

    init_F_arr();

    uint8_t s_cells[7];
    uint8_t t_cells[7];

    for (uint16_t s = 0; s < MAX_STATE; ++s) {
        /* decode state into cells[0..6] (each 2 bits) */
        uint16_t tmp = s;
        for (int i = 0; i < 7; ++i) {
            s_cells[i] = tmp & 3;
            tmp >>= 2;
        }

        for (int K = 0; K < 4; ++K) {
            for (int t0 = 0; t0 < 4; ++t0) {
                t_cells[0] = (uint8_t)t0;
                dfs_gen(K, s_cells, t_cells, 0, s);
            }
        }
    }
}

/* ------------------------------------------------------------------------- */
/* DP arrays: cur[K][state], next[K][state] */
static seq_t cur[4][MAX_STATE];
static seq_t nxt[4][MAX_STATE];

/* ------------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* definition starts at offset 1 */

    init_adjacency();

    /* initialize for first row: one way for each state */
    for (int K = 0; K < 4; ++K)
        for (int s = 0; s < MAX_STATE; ++s)
            cur[K][s] = (seq_t)1;

    /* iterate through n transitions (n rows after the first) */
    for (idx_t step = 0; step < n; ++step) {
        /* clear next arrays */
        for (int K = 0; K < 4; ++K)
            for (int s = 0; s < MAX_STATE; ++s)
                nxt[K][s] = (seq_t)0;

        for (int K = 0; K < 4; ++K) {
            for (int s = 0; s < MAX_STATE; ++s) {
                seq_t val = cur[K][s];
                if (val == 0)
                    continue;
                StateAdj *st = &adj[K][s];
                for (uint16_t i = 0; i < st->len; ++i) {
                    nxt[K][st->data[i]] += val;
                }
            }
        }

        /* copy next to cur for next iteration */
        for (int K = 0; K < 4; ++K)
            for (int s = 0; s < MAX_STATE; ++s)
                cur[K][s] = nxt[K][s];
    }

    /* sum over all states and all K */
    seq_t total = (seq_t)0;
    for (int K = 0; K < 4; ++K)
        for (int s = 0; s < MAX_STATE; ++s)
            total += cur[K][s];

    return total;
}
