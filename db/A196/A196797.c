/*
 * A196797  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 3,2,0,4,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A196797
 * Formula: dynamic programming on rows of width 3 over alphabet {0,1,2,3,4}
 * each value x equals the number of neighbors (horizontal + vertical)
 * that equal f[x] where f = {3,2,0,4,1}.
 * The implementation builds a state graph of row pairs and counts
 * paths of length n.  All arrays are allocated once and reused; the
 * function works for any idx_t >= 1 and any seq_t with the required
 * arithmetic operations.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* constants ------------------------------------------------------------- */
#define NUM_COLS 3
#define NUM_ROWS 125            /* 5^3 possible rows */
#define NULL_IDX NUM_ROWS       /* sentinel for missing neighbour */

/* mapping f : x -> number of neighbors that must equal f[x] */
static const int fmap[5] = {3, 2, 0, 4, 1};

/* row data: rows[idx][c] = value in column c (0..4) */
static int rows[NUM_ROWS][NUM_COLS];

/* horizontal matches in a row: horiz[idx][c] = number of neighbours in
   same row equal to fmap[value] */
static int horiz[NUM_ROWS][NUM_COLS];

/* adjacency data for the state graph:
   states are encoded as s = prev * NUM_ROWS + curr
   adjOffset[s] ... first outgoing edge index in adjTarget
   adjCount[s]  ... number of outgoing edges from state s
   adjTarget[...] contains encoded destination state indices */
static int *adjOffset = NULL;
static int *adjCount  = NULL;
static int *adjTarget = NULL;
static size_t NUM_STATES = (size_t)NUM_ROWS * NUM_ROWS;

/* ------------------------------------------------------------------ */
/* Test whether a triplet of rows (prev,curr,next) satisfies the
   neighbour‑count condition for every cell in the middle row. */
static inline bool isValidIdx(int prevIdx, int currIdx, int nextIdx)
{
    const int *cur = rows[currIdx];
    for (int c = 0; c < NUM_COLS; ++c) {
        int val   = cur[c];
        int fval  = fmap[val];
        int h     = horiz[currIdx][c];
        int up    = (prevIdx == NULL_IDX) ? 0 :
                   (rows[prevIdx][c] == fval);
        int down  = (nextIdx == NULL_IDX) ? 0 :
                   (rows[nextIdx][c] == fval);
        if (h + up + down != val)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Build tables of rows, horizontals and transitions between states. */
static void init_lookup(void)
{
    /* fill rows array */
    for (int idx = 0; idx < NUM_ROWS; ++idx) {
        int rem = idx;
        rows[idx][0] = rem / 25;
        rem %= 25;
        rows[idx][1] = rem / 5;
        rows[idx][2] = rem % 5;
    }

    /* compute horizontal matches for each row */
    for (int idx = 0; idx < NUM_ROWS; ++idx) {
        for (int c = 0; c < NUM_COLS; ++c) {
            int val   = rows[idx][c];
            int fval  = fmap[val];
            int h = 0;
            if (c > 0   && rows[idx][c-1] == fval) ++h;
            if (c < 2   && rows[idx][c+1] == fval) ++h;
            horiz[idx][c] = h;
        }
    }

    /* allocate adjacency arrays */
    adjCount  = (int *)calloc(NUM_STATES, sizeof(int));
    adjOffset = (int *)calloc(NUM_STATES + 1, sizeof(int));

    /* first pass: count edges */
    for (int prev = 0; prev < NUM_ROWS; ++prev) {
        for (int curr = 0; curr < NUM_ROWS; ++curr) {
            size_t s = (size_t)prev * NUM_ROWS + curr;
            int cnt = 0;
            for (int next = 0; next < NUM_ROWS; ++next)
                if (isValidIdx(prev, curr, next))
                    ++cnt;
            adjCount[s] = cnt;
        }
    }

    /* build prefix sums for offsets */
    for (size_t s = 0; s < NUM_STATES; ++s)
        adjOffset[s + 1] = adjOffset[s] + adjCount[s];

    size_t totalEdges = adjOffset[NUM_STATES];
    adjTarget = (int *)malloc(totalEdges * sizeof(int));

    /* second pass: fill adjacency list */
    int *ptr = adjTarget;
    for (int prev = 0; prev < NUM_ROWS; ++prev) {
        for (int curr = 0; curr < NUM_ROWS; ++curr) {
            size_t s = (size_t)prev * NUM_ROWS + curr;
            for (int next = 0; next < NUM_ROWS; ++next)
                if (isValidIdx(prev, curr, next))
                    *ptr++ = (int)((size_t)curr * NUM_ROWS + next);
        }
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_lookup();
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;          /* out of OEIS range – return 0 */

    /* n = 1: single row */
    if (n == 1) {
        seq_t result = (seq_t)0;
        for (int r = 0; r < NUM_ROWS; ++r)
            if (isValidIdx(NULL_IDX, r, NULL_IDX))
                ++result;
        return result;
    }

    /* allocate DP arrays */
    size_t states = NUM_STATES;
    seq_t *dp    = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(states, sizeof(seq_t));

    /* initialise with first two rows (prev = NULL, curr = r1) */
    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            if (isValidIdx(NULL_IDX, r1, r2))
                dp[(size_t)r1 * NUM_ROWS + r2] = (seq_t)1;
        }
    }

    /* n = 2: finish after two rows */
    if (n == 2) {
        seq_t result = (seq_t)0;
        for (size_t s = 0; s < states; ++s) {
            if (dp[s] == (seq_t)0) continue;
            int prev = (int)(s / NUM_ROWS);
            int curr = (int)(s % NUM_ROWS);
            if (isValidIdx(prev, curr, NULL_IDX))
                result += dp[s];
        }
        free(dp);
        free(newdp);
        return result;
    }

    /* DP for rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* reset newdp */
        for (size_t i = 0; i < states; ++i)
            newdp[i] = (seq_t)0;

        for (size_t s = 0; s < states; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            size_t start = adjOffset[s];
            int    cnt   = adjCount[s];
            for (int k = 0; k < cnt; ++k) {
                size_t t = (size_t)adjTarget[start + k];
                newdp[t] += val;
            }
        }

        /* swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* final validation for the last row */
    seq_t result = (seq_t)0;
    for (size_t s = 0; s < states; ++s) {
        if (dp[s] == (seq_t)0) continue;
        int prev = (int)(s / NUM_ROWS);
        int curr = (int)(s % NUM_ROWS);
        if (isValidIdx(prev, curr, NULL_IDX))
            result += dp[s];
    }

    free(dp);
    free(newdp);
    return result;
}
