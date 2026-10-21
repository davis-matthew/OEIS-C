/*
 * A234408  Number of (n+1)X(3+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 12
 *          and no adjacent elements equal.
 * https://oeis.org/A234408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+1)×4 arrays with entries in 0..6
 * such that
 *   1. No two orthogonally adjacent cells are equal.
 *   2. Every 2×2 sub‑block has
 *          |a00‑a01| + |a10‑a11| + |a00‑a10| + |a01‑a11| = 12
 * where each axy is the value in that cell.
 *
 * The algorithm enumerates all row patterns of length 4 that satisfy the
 * horizontal adjacency rule, builds a directed graph of admissible
 * transitions between rows, and counts walks of length n over this graph.
 * The DP uses two vectors of type seq_t.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h> /* for abs */

#define NUM_STATES 1512  /* 7 * 6^3 */

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} IntVec;

/* Push value into IntVec, reallocating as necessary */
static void vec_push(IntVec *v, int value)
{
    if (v->len == v->cap) {
        size_t new_cap = v->cap == 0 ? 4 : v->cap * 2;
        int *new_data = (int *)realloc(v->data, new_cap * sizeof(int));
        if (!new_data) abort();
        v->data = new_data;
        v->cap = new_cap;
    }
    v->data[v->len++] = value;
}

static int state_vals[NUM_STATES][4];
static int state_index[7*7*7*7];   /* mapping from 4‑digit base‑7 code to state index */
static IntVec transitions[NUM_STATES];
static bool initialized = false;

/* Build the set of valid row states and the state transition graph */
static void init(void)
{
    if (initialized) return;
    for (int i = 0; i < 7*7*7*7; ++i) state_index[i] = -1;
    int idx = 0;

    /* Enumerate all 4‑tuples with no horizontal repetition */
    for (int a0 = 0; a0 <= 6; ++a0)
    for (int a1 = 0; a1 <= 6; ++a1) if (a1 != a0)
    for (int a2 = 0; a2 <= 6; ++a2) if (a2 != a1)
    for (int a3 = 0; a3 <= 6; ++a3) if (a3 != a2) {
        state_vals[idx][0] = a0;
        state_vals[idx][1] = a1;
        state_vals[idx][2] = a2;
        state_vals[idx][3] = a3;
        int code = (((a0*7)+a1)*7 + a2)*7 + a3;
        state_index[code] = idx;
        ++idx;
    }

    /* Build adjacency lists */
    for (int i = 0; i < NUM_STATES; ++i) {
        const int *a = state_vals[i];
        for (int j = 0; j < NUM_STATES; ++j) {
            const int *b = state_vals[j];

            /* Vertical adjacency: no equal elements side‑by‑side */
            if (a[0] == b[0] || a[1] == b[1] || a[2] == b[2] || a[3] == b[3])
                continue;

            /* 2×2 block constraints */
            int s;
            /* column 0 */
            s = abs(a[0]-a[1]) + abs(b[0]-b[1]) + abs(a[0]-b[0]) + abs(a[1]-b[1]);
            if (s != 12) continue;
            /* column 1 */
            s = abs(a[1]-a[2]) + abs(b[1]-b[2]) + abs(a[1]-b[1]) + abs(a[2]-b[2]);
            if (s != 12) continue;
            /* column 2 */
            s = abs(a[2]-a[3]) + abs(b[2]-b[3]) + abs(a[2]-b[2]) + abs(a[3]-b[3]);
            if (s != 12) continue;

            vec_push(&transitions[i], j);
        }
    }

    initialized = true;
}

/*
 * Compute a(n) for n >= 1.
 * The grid has n+1 rows (indexed 0..n).  We start with any valid first row.
 * A walk of length n in the transition graph corresponds to a complete grid.
 */
seq_t a(idx_t n)
{
    init();

    static seq_t *dp[2];
    static bool dp_allocated = false;
    if (!dp_allocated) {
        dp[0] = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
        dp[1] = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
        dp_allocated = true;
    }

    seq_t *cur = dp[0];
    seq_t *next = dp[1];

    /* initialise counts for the first row */
    for (int i = 0; i < NUM_STATES; ++i) cur[i] = 1;

    /* Transition through n rows (n steps) */
    for (idx_t step = 1; step <= n; ++step) {
        /* clear next */
        for (int i = 0; i < NUM_STATES; ++i) next[i] = 0;

        for (int i = 0; i < NUM_STATES; ++i) {
            if (cur[i] == 0) continue;
            seq_t val = cur[i];
            IntVec *vec = &transitions[i];
            for (size_t k = 0; k < vec->len; ++k) {
                int j = vec->data[k];
                next[j] += val;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum over all possible last rows */
    seq_t result = 0;
    for (int i = 0; i < NUM_STATES; ++i) result += cur[i];
    return result;
}
