/*
 * A148308  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 0,
 *          1), (0, 1, 1), (1, 0, -1)}.
 * https://oeis.org/A148308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A148308: Number of walks of length n in N³ starting at (0,0,0)
 *  using only the steps {(-1,-1,1),(-1,0,1),(0,1,1),(1,0,-1)} and
 *  staying in the non‑negative octant.
 *  The routine uses dynamic programming over reachable states and
 *  memoises results for arbitrary call order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/ensure_capacity.h"

typedef struct {
    uint64_t *keys;          /* (x<<32)|(y<<16)|z  */
    seq_t    *vals;
    unsigned char *used;     /* 0/1 for each bucket */
    size_t   *used_idx;      /* indices of used buckets */
    size_t   used_cnt;       /* number of used buckets */
    size_t   cap;            /* capacity (power of two) */
    size_t   mask;           /* cap-1 for masking */
} StateTable;

#define INIT_CAP 262144 /* must be power of two */

static StateTable cur, nxt;
static seq_t *results = NULL;
static size_t results_cap = 0;
static idx_t max_comp = 0;  /* highest n for which results[] is filled */

/* initialisation of the two hash tables and the start state */
static void init_tables(void)
{
    size_t c = INIT_CAP;
    cur.cap   = c; cur.mask  = c-1;
    cur.keys  = (uint64_t *)malloc(c * sizeof(uint64_t));
    cur.vals  = (seq_t   *)malloc(c * sizeof(seq_t));
    cur.used  = (unsigned char *)malloc(c);
    cur.used_idx = (size_t *)malloc(c * sizeof(size_t));
    cur.used_cnt = 0;

    nxt.cap   = c; nxt.mask  = c-1;
    nxt.keys  = (uint64_t *)malloc(c * sizeof(uint64_t));
    nxt.vals  = (seq_t   *)malloc(c * sizeof(seq_t));
    nxt.used  = (unsigned char *)malloc(c);
    nxt.used_idx = (size_t *)malloc(c * sizeof(size_t));
    nxt.used_cnt = 0;

    /* start state (0,0,0) has count 1 */
    cur.keys[0] = 0;
    cur.vals[0] = (seq_t)1;
    cur.used[0] = 1;
    cur.used_idx[0] = 0;
    cur.used_cnt = 1;
}

/* add a key/value pair to a table, summing if key already exists */
static void add_to_table(StateTable *t, uint64_t key, seq_t val)
{
    size_t h = key & t->mask;
    while (t->used[h]) {
        if (t->keys[h] == key) {
            t->vals[h] += val;
            return;
        }
        h = (h + 1) & t->mask;
    }
    t->used[h]      = 1;
    t->keys[h]      = key;
    t->vals[h]      = val;
    t->used_idx[t->used_cnt++] = h;
}

/* clear all entries in a table */
static void clear_table(StateTable *t)
{
    for (size_t i = 0; i < t->used_cnt; ++i)
        t->used[t->used_idx[i]] = 0;
    t->used_cnt = 0;
}

/* the requested sequence entry */
seq_t a(idx_t n)
{
    static bool tables_inited = false;
    if (!tables_inited) {
        init_tables();
        tables_inited = true;
    }

    /* ensure results array large enough */
    if (results_cap <= (size_t)n) {
        ensure_capacity((size_t)n + 1, &results_cap,
                        (void **)&results, sizeof(seq_t));
        /* initialise the first term if array is newly allocated */
        if (results_cap == 0)
            results[0] = (seq_t)1;
    }

    /* zero length walk */
    if (n == 0)
        return (seq_t)1;

    /* compute missing values up to requested n */
    for (idx_t t = max_comp + 1; t <= n; ++t) {
        clear_table(&nxt);
        seq_t total = 0;

        for (size_t idx = 0; idx < cur.used_cnt; ++idx) {
            size_t pos = cur.used_idx[idx];
            uint64_t key = cur.keys[pos];
            seq_t    cnt = cur.vals[pos];

            int x = (int)(key >> 32);
            int y = (int)((key >> 16) & 0xFFFF);
            int z = (int)(key & 0xFFFF);

            /* step (-1,-1,+1) */
            if (x >= 1 && y >= 1) {
                uint64_t nk = ((uint64_t)(x-1) << 32) |
                              ((uint64_t)(y-1) << 16) |
                               (uint64_t)(z+1);
                add_to_table(&nxt, nk, cnt);
            }
            /* step (-1,0,+1) */
            if (x >= 1) {
                uint64_t nk = ((uint64_t)(x-1) << 32) |
                              ((uint64_t)y << 16) |
                               (uint64_t)(z+1);
                add_to_table(&nxt, nk, cnt);
            }
            /* step (0,1,+1) – no pre‑condition needed */
            {
                uint64_t nk = ((uint64_t)x << 32) |
                              ((uint64_t)(y+1) << 16) |
                               (uint64_t)(z+1);
                add_to_table(&nxt, nk, cnt);
            }
            /* step (1,0,-1) */
            if (z >= 1) {
                uint64_t nk = ((uint64_t)(x+1) << 32) |
                              ((uint64_t)y << 16) |
                               (uint64_t)(z-1);
                add_to_table(&nxt, nk, cnt);
            }
        }

        /* sum all partial counts to get total walks of length t */
        for (size_t i = 0; i < nxt.used_cnt; ++i)
            total += nxt.vals[nxt.used_idx[i]];

        results[t] = total;

        /* swap cur and nxt for next iteration */
        StateTable tmp = cur;
        cur = nxt;
        nxt = tmp;

        max_comp = t;
    }

    return results[n];
}
