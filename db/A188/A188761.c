/*
 * A188761  Number of nX7 binary arrays without the pattern 0 0 1 vertically
 *          or horizontally.
 * https://oeis.org/A188761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding the pattern 0 0 1
 *            vertically or horizontally, computed by a transfer‑matrix DP
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------- local static data -------------------- */

static uint8_t *valid_rows = NULL;    /* list of 7‑bit rows w/o horizontal 001 */
static size_t V = 0;                  /* number of such rows */

static size_t S = 0;                  /* number of states (prevprev,row) */

static int *adj_head = NULL;          /* head of adjacency list of each state   */
static int *adj_next = NULL;          /* next edge in adjacency list            */
static int *adj_to   = NULL;          /* target state of each edge              */

static seq_t *dp_cur  = NULL;         /* DP vector for the current n            */
static seq_t *dp_next = NULL;         /* DP vector for the next n               */

static idx_t  max_n_computed = 2;     /* highest n for which dp_cur is valid     */

/* -------------------- helper: build transfer matrix -------------------- */

static void build_transfer_graph(void)
{
    if (valid_rows != NULL)
        return;                       /* already built */

    /* 1. Enumerate all 7‑bit rows that do NOT contain the horizontal pattern 0 0 1. */
    for (unsigned mask = 0; mask < 128; ++mask) {
        bool ok = true;
        for (int pos = 0; pos <= 4; ++pos) {
            if (((mask >> pos)     & 1u) == 0 &&
                ((mask >> (pos+1)) & 1u) == 0 &&
                ((mask >> (pos+2)) & 1u) == 1u) {
                ok = false; break;
            }
        }
        if (ok) {
            uint8_t *tmp = (uint8_t*)realloc(valid_rows, (V+1)*sizeof(uint8_t));
            if (!tmp) { free(valid_rows); return; }
            valid_rows = tmp;
            valid_rows[V++] = (uint8_t)mask;
        }
    }

    /* 2. Number of states and rough estimate of edge count. */
    S = V * V;
    size_t est_edges = 0;
    for (size_t a = 0; a < V; ++a) {
        uint8_t a_bits = valid_rows[a];
        for (size_t b = 0; b < V; ++b) {
            uint8_t b_bits = valid_rows[b];
            uint8_t forbid = ~(a_bits | b_bits) & 0x7Fu;
            for (size_t c = 0; c < V; ++c) {
                if ((valid_rows[c] & forbid) == 0)
                    ++est_edges;
            }
        }
    }

    /* 3. Allocate adjacency arrays. */
    adj_head = (int*)malloc(S * sizeof(int));
    adj_next = (int*)malloc(est_edges * sizeof(int));
    adj_to   = (int*)malloc(est_edges * sizeof(int));
    if (!adj_head || !adj_next || !adj_to) { return; }

    /* 4. Build adjacency lists. */
    size_t e = 0;
    for (size_t a = 0; a < V; ++a) {
        uint8_t a_bits = valid_rows[a];
        for (size_t b = 0; b < V; ++b) {
            uint8_t b_bits = valid_rows[b];
            size_t s = a * V + b;   /* current state (prevprev,prev) */
            adj_head[s] = -1;
            uint8_t forbid = ~(a_bits | b_bits) & 0x7Fu;
            for (size_t c = 0; c < V; ++c) {
                if ((valid_rows[c] & forbid) == 0) {
                    adj_next[e] = adj_head[s];
                    adj_to[e]   = (int)(b * V + c);
                    adj_head[s] = (int)e;
                    ++e;
                }
            }
        }
    }

    /* 5. Allocate DP buffers. */
    dp_cur  = (seq_t*)calloc(S, sizeof(seq_t));
    dp_next = (seq_t*)calloc(S, sizeof(seq_t));
    if (!dp_cur || !dp_next) { return; }

    /* 6. Initial DP for n = 2: every pair of valid rows is allowed. */
    for (size_t a = 0; a < V; ++a)
        for (size_t b = 0; b < V; ++b)
            dp_cur[a * V + b] = (seq_t)1;

    max_n_computed = 2;
}

/* -------------------- main sequence function -------------------- */

seq_t a(idx_t n)
{
    build_transfer_graph();

    if (n == 1)                 /* only horizontal restriction applies */
        return (seq_t)V;

    /* n >= 2 : dp_cur holds counts for the last two rows. */
    if (n > max_n_computed) {
        for (idx_t step = max_n_computed + 1; step <= n; ++step) {
            /* zero dp_next */
            for (size_t i = 0; i < S; ++i)
                dp_next[i] = (seq_t)0;

            /* transition */
            for (size_t s = 0; s < S; ++s) {
                seq_t val = dp_cur[s];
                if (val == (seq_t)0) continue;
                for (int e = adj_head[s]; e != -1; e = adj_next[e]) {
                    dp_next[adj_to[e]] += val;
                }
            }

            /* swap buffers */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }
        max_n_computed = n;
    }

    /* sum over final states to get total number of arrays */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < S; ++i)
        total += dp_cur[i];

    return total;
}
