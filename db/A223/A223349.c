/*
 * A223349  3X3X3 triangular graph without horizontal edges coloring a
 *          rectangular array: number of nX5 0..5 arrays where 0..5 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 and every
 *          array movement to a horizontal or vertical neighbor moves along an
 *          edge of this graph.
 * https://oeis.org/A223349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computed by transfer‑matrix dynamic programming over the 5‑cell
 *            rows of the graph with edges (0‑1), (0‑2), (1‑3), (1‑4), (2‑4),
 *            (2‑5). Two rows are compatible if every vertical pair forms an
 *            edge of the graph
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No simple closed form is known; the recurrence below gives the
   sequence for all n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool _a223349_initialized = false;

static unsigned char *_a223349_states = NULL;   /* concatenated 5‑label states */
static int *_a223349_start   = NULL;           /* start indices in adjacency list */
static int *_a223349_next_idx = NULL;          /* flattened adjacency list */

static seq_t *_a223349_dp   = NULL;            /* current DP values */
static seq_t *_a223349_next = NULL;            /* next DP values */
static int   _a223349_S     = 0;               /* number of valid states */

/* ------------------------------------------------------------------ */
/* Initialization of all data structures needed for the DP           */
/* ------------------------------------------------------------------ */
static void _a223349_init(void)
{
    if (_a223349_initialized)
        return;

    /* adjacency matrix of the target graph */
    bool adj[6][6] = {0};
    adj[0][1] = adj[1][0] = true;
    adj[0][2] = adj[2][0] = true;
    adj[1][3] = adj[3][1] = true;
    adj[1][4] = adj[4][1] = true;
    adj[2][4] = adj[4][2] = true;
    adj[2][5] = adj[5][2] = true;

    /* Enumerate all 5‑tuples satisfying horizontal edge constraints. */
    const int max_code = 6 * 6 * 6 * 6 * 6; /* 7776 */
    int *code_to_id = (int*)malloc(max_code * sizeof(int));
    for (int i = 0; i < max_code; ++i)
        code_to_id[i] = -1;

    unsigned char *tmp_states = (unsigned char*)malloc(max_code * 5 * sizeof(unsigned char));
    int state_count = 0;

    for (int code = 0; code < max_code; ++code) {
        unsigned char lab[5];
        int t = code;
        for (int k = 0; k < 5; ++k) {
            lab[k] = t % 6;
            t /= 6;
        }
        bool ok = true;
        for (int k = 0; k < 4; ++k) {
            if (!adj[lab[k]][lab[k + 1]]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            code_to_id[code] = state_count;
            memcpy(tmp_states + state_count * 5, lab, 5);
            ++state_count;
        }
    }

    _a223349_S = state_count;
    _a223349_states = (unsigned char*)malloc(state_count * 5 * sizeof(unsigned char));
    memcpy(_a223349_states, tmp_states, state_count * 5 * sizeof(unsigned char));
    free(tmp_states);
    free(code_to_id);

    /* Build adjacency list for vertical transitions. */
    _a223349_start = (int*)malloc((state_count + 1) * sizeof(int));
    int total_edges = 0;
    /* Allocate worst case: all pairs possible. */
    int *edges = (int*)malloc(state_count * state_count * sizeof(int));

    for (int i = 0; i < state_count; ++i) {
        _a223349_start[i] = total_edges;
        for (int j = 0; j < state_count; ++j) {
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                if (!adj[_a223349_states[i * 5 + k]][_a223349_states[j * 5 + k]]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                edges[total_edges++] = j;
            }
        }
    }
    _a223349_start[state_count] = total_edges;  /* sentinel */
    _a223349_next_idx = edges;

    /* Allocate DP buffers. */
    _a223349_dp   = (seq_t*)calloc(state_count, sizeof(seq_t));
    _a223349_next = (seq_t*)calloc(state_count, sizeof(seq_t));

    _a223349_initialized = true;
}

/* ------------------------------------------------------------------ */
/* seq_t a(idx_t n)                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    _a223349_init();

    if (n < 1)
        return (seq_t)0;

    /* n == 1: just the number of admissible 1×5 arrays. */
    if (n == 1)
        return (seq_t)_a223349_S;

    /* initialize dp to 1 for each valid first row */
    for (int i = 0; i < _a223349_S; ++i)
        _a223349_dp[i] = (seq_t)1;

    /* iterate over the remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* zero next array */
        for (int i = 0; i < _a223349_S; ++i)
            _a223349_next[i] = (seq_t)0;

        for (int i = 0; i < _a223349_S; ++i) {
            seq_t val = _a223349_dp[i];
            if (val == (seq_t)0)
                continue;
            int start = _a223349_start[i];
            int end   = _a223349_start[i + 1];
            for (int idx = start; idx < end; ++idx) {
                int j = _a223349_next_idx[idx];
                _a223349_next[j] += val;
            }
        }
        /* swap dp and next for the next iteration */
        seq_t *tmp = _a223349_dp;
        _a223349_dp = _a223349_next;
        _a223349_next = tmp;
    }

    /* sum over all final row states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < _a223349_S; ++i)
        result += _a223349_dp[i];

    return result;
}
