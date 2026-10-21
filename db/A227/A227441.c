/*
 * A227441  Number of n X 5 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 6 binary array having two adjacent 1's and two
 *          adjacent 0's.
 * https://oeis.org/A227441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays that can be obtained as
 *            indicator arrays of 2×2 subblocks of an (n+1)×6 binary matrix
 *            having two adjacent 1's and two adjacent 0's. Computed by a
 *            dynamic‑programming algorithm on a finite automaton that tracks
 *            the set of possible last rows of the big matrix
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   ctzll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ctzll.h"

/* ---------- auxiliary bit‑count helper ---------- */
#if defined(__GNUC__) || defined(__clang__)

#else

#endif

/* ---------- local data structures ---------- */
typedef struct Edge {
    uint8_t mask;    /* 0..31 */
    int target;      /* index of target state */
} Edge;

typedef struct State {
    uint64_t subset;            /* set of rows (bitmask) */
    Edge edges[32];             /* outgoing edges (max 32 per state) */
    uint8_t edge_count;         /* current number of edges */
} State;

/* ---------- globals for the automaton ---------- */
static bool graph_built = false;
static State *states        = NULL;
static size_t state_count   = 0;
static size_t state_cap     = 0;
static uint8_t pair_mask[64][64];
static uint64_t dest_rows[64][32];

/* ---------- build the state graph once ---------- */
static void build_graph(void)
{
    /* precompute pair → mask */
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            uint8_t m = 0;
            for (int j = 0; j < 5; ++j) {
                uint8_t a0 = (x >> j) & 1;
                uint8_t a1 = (x >> (j + 1)) & 1;
                uint8_t b0 = (y >> j) & 1;
                uint8_t b1 = (y >> (j + 1)) & 1;
                int horizontal = (a0 == a1) && (b0 == b1) && (a0 != b0);
                int vertical   = (a0 == b0) && (a1 == b1) && (a0 != a1);
                if (horizontal || vertical)
                    m |= 1u << j;
            }
            pair_mask[x][y] = m;
        }
    }

    /* precompute destination rows for each (source,mask) pair */
    for (int x = 0; x < 64; ++x) {
        for (int m = 0; m < 32; ++m) dest_rows[x][m] = 0;
        for (int y = 0; y < 64; ++y) {
            uint8_t m = pair_mask[x][y];
            dest_rows[x][m] |= ((uint64_t)1 << y);
        }
    }

    /* setup dynamic arrays */
    state_cap = 64;
    states = (State *)malloc(state_cap * sizeof(State));
    if (!states) exit(1);

    /* small queue for BFS */
    size_t queue_cap = 64;
    int *queue = (int *)malloc(queue_cap * sizeof(int));
    if (!queue) exit(1);
    size_t head = 0, tail = 0;

    /* initial state: all 64 rows possible as first row */
    states[0].subset = UINT64_MAX;     /* all bits set */
    states[0].edge_count = 0;
    state_count = 1;
    queue[tail++] = 0;

    while (head < tail) {
        int idx = queue[head++];
        uint64_t S = states[idx].subset;

        /* for each possible mask, compute next subset */
        for (int m = 0; m < 32; ++m) {
            uint64_t dest = 0;
            uint64_t tmp = S;
            while (tmp) {
                int x = ctzll(tmp);
                tmp &= tmp - 1;
                dest |= dest_rows[x][m];
            }
            if (dest == 0) continue;          /* no transition with this mask */

            /* find or create target state */
            int dest_idx = -1;
            for (size_t k = 0; k < state_count; ++k)
                if (states[k].subset == dest) { dest_idx = (int)k; break; }

            if (dest_idx == -1) {
                /* grow states array if necessary */
                if (state_count == state_cap) {
                    state_cap *= 2;
                    states = (State *)realloc(states, state_cap * sizeof(State));
                    if (!states) exit(1);
                }
                dest_idx = (int)state_count;
                states[dest_idx].subset = dest;
                states[dest_idx].edge_count = 0;
                ++state_count;
                /* grow queue if necessary */
                if (tail == queue_cap) {
                    queue_cap *= 2;
                    queue = (int *)realloc(queue, queue_cap * sizeof(int));
                    if (!queue) exit(1);
                }
                queue[tail++] = dest_idx;
            }

            /* add edge from idx to dest_idx */
            if (states[idx].edge_count < 32) {
                states[idx].edges[states[idx].edge_count].mask    = (uint8_t)m;
                states[idx].edges[states[idx].edge_count].target = dest_idx;
                ++states[idx].edge_count;
            }
        }
    }

    free(queue);
    graph_built = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!graph_built)
        build_graph();

    const size_t M = state_count;
    seq_t *dp_cur  = (seq_t *)calloc(M, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(M, sizeof(seq_t));
    if (!dp_cur || !dp_next) exit(1);

    dp_cur[0] = (seq_t)1;      /* start with the initial state */

    for (idx_t step = 0; step < n; ++step) {
        /* reset dp_next to zero */
        memset(dp_next, 0, M * sizeof(seq_t));

        for (size_t i = 0; i < M; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0) continue;
            const State *s = &states[i];
            for (uint8_t e = 0; e < s->edge_count; ++e) {
                int tgt = s->edges[e].target;
                dp_next[tgt] += val;   /* each edge corresponds to a distinct mask */
            }
        }
        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < M; ++i)
        result += dp_cur[i];

    free(dp_cur);
    free(dp_next);
    return result;
}
