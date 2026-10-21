/*
 * A208564  Number of nX5 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Relation: This implementation uses a transfer‑matrix dynamic programming
   algorithm.  Each state represents the previous row of the n×5 array
   together with two flags: whether a 1 and/or a 2 have appeared so far
   (in row‑major order).  Transitions generate all possible next rows
   that keep the “at most one equal neighbour” condition.  The method
   works for any n ≥ 1 and uncached values are computed on demand. */

#include <stdlib.h>
#include <string.h>

#define ROWS          243          /* 3^5 possible rows */
#define STATE_SIZE    (ROWS * 4)    /* 4 flag combinations: (has1,has2) */
#define MAX_EDGES     ROWS          /* at most one transition per target row */

static int row_digits[ROWS][5];
static int adj[STATE_SIZE][MAX_EDGES];
static int adj_count[STATE_SIZE];

static seq_t *cur  = NULL;   /* distribution after current row          */
static seq_t *next = NULL;   /* auxiliary array for updates            */
static seq_t *cache      = NULL;   /* cached a(n) values, cache[i] = a(i+1) */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static int    max_computed = 0;   /* last n that has been fully computed   */
static int    initialized  = 0;   /* 0‑initialisation guard                */

/* precompute the 243 row patterns and the transition adjacency list */
static void build_state(void)
{
    int pow3[5];
    int i, c;

    pow3[0] = 1;
    for (c = 1; c < 5; ++c) pow3[c] = pow3[c-1] * 3;

    /* decompose each integer 0..242 into 5 base‑3 digits */
    for (i = 0; i < ROWS; ++i) {
        int x = i;
        for (c = 0; c < 5; ++c) {
            row_digits[i][c] = x % 3;
            x /= 3;
        }
    }

    /* clear adjacency counts */
    for (i = 0; i < STATE_SIZE; ++i)
        adj_count[i] = 0;

    /* build transitions between all possible row pairs */
    for (int prev = 0; prev < ROWS; ++prev) {
        const int *prev_vals = row_digits[prev];
        for (int curr = 0; curr < ROWS; ++curr) {
            const int *curr_vals = row_digits[curr];
            for (int has1 = 0; has1 <= 1; ++has1) {
                for (int has2 = 0; has2 <= 1; ++has2) {
                    if (has2 && !has1) continue;          /* 2 cannot precede 1 */

                    int lhas1 = has1, lhas2 = has2;
                    int ok = 1;

                    for (c = 0; c < 5; ++c) {
                        int val = curr_vals[c];
                        int same = 0;

                        if (c > 0 && val == curr_vals[c-1]) ++same;  /* left   */
                        if (prev_vals[c] == val)          ++same;  /* up     */
                        if (c > 0 && prev_vals[c-1] == val) ++same;  /* up‑left */

                        if (same > 1) { ok = 0; break; }

                        /* handle introduction of 1 or 2 */
                        if (val == 1) lhas1 = 1;
                        else if (val == 2) {
                            if (!lhas1) { ok = 0; break; }
                            lhas2 = 1;
                        }
                    }

                    if (!ok) continue;

                    int curr_state = ((has1 << 1) | has2) * ROWS + prev;
                    int next_state = ((lhas1 << 1) | lhas2) * ROWS + curr;
                    int idx = adj_count[curr_state]++;
                    adj[curr_state][idx] = next_state;
                }
            }
        }
    }

    /* allocate DP arrays */
    cur  = (seq_t *)calloc(STATE_SIZE, sizeof(seq_t));
    next = (seq_t *)calloc(STATE_SIZE, sizeof(seq_t));

    /* init: first row must start with 0 (replaces empty array) */
    for (int curr = 0; curr < ROWS; ++curr) {
        const int *vals = row_digits[curr];
        if (vals[0] != 0) continue;          /* first element must be 0 */

        int has1 = 0, has2 = 0;
        int ok = 1;

        for (c = 0; c < 5; ++c) {
            int v = vals[c];
            if (v == 1) has1 = 1;
            else if (v == 2) {
                if (!has1) { ok = 0; break; }
                has2 = 1;
            }
        }
        if (!ok) continue;

        int state = ((has1 << 1) | has2) * ROWS + curr;
        cur[state] += (seq_t)1;
    }

    /* cache a(1) */
    seq_t sum = 0;
    for (i = 0; i < STATE_SIZE; ++i) sum += cur[i];

    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = sum;
    cache_len = 1;
    max_computed = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_state();
        initialized = 1;
    }

    if (n <= (idx_t)max_computed)
        return cache[(size_t)n - 1];

    for (size_t row = max_computed + 1; row <= (size_t)n; ++row) {
        memset(next, 0, STATE_SIZE * sizeof(seq_t));

        for (size_t st = 0; st < STATE_SIZE; ++st) {
            if (cur[st] == (seq_t)0) continue;
            int outcnt = adj_count[st];
            int *out = adj[st];
            for (int j = 0; j < outcnt; ++j)
                next[out[j]] += cur[st];
        }

        seq_t *tmp = cur;
        cur = next;
        next = tmp;

        seq_t sum = 0;
        for (size_t st = 0; st < STATE_SIZE; ++st) sum += cur[st];

        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[cache_len++] = sum;
    }

    max_computed = (int)n;
    return cache[(size_t)n - 1];
}
