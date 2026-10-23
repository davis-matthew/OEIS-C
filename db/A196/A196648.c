/*
 * A196648  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,4,0,2,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A196648: number of n×3 arrays over {0,1,2,3,4} such that for each cell x
   the number of horizontal and vertical neighbors equal to mapv[x] (mapv = {3,4,0,2,1}) is exactly x.
   Computed by dynamic programming over the rows of the array.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* for memset */

#define ROWS 125                 /* number of possible rows (5^3) */
#define PAIRS (ROWS*ROWS)        /* number of ordered row pairs */

static int rows[ROWS][3];        /* all possible rows */
static const int mapv[5] = {3,4,0,2,1};

/* Count how many neighbors of each cell equal to its required value.
   Returns 1 if the row is valid for the given up and down rows, 0 otherwise.
   up/down may be -1 meaning absent. */
static inline bool is_valid_row(int cur, int up, int down)
{
    int c0 = rows[cur][0];
    int c1 = rows[cur][1];
    int c2 = rows[cur][2];

    int mv0 = mapv[c0];
    int mv1 = mapv[c1];
    int mv2 = mapv[c2];

    int cnt0 = 0, cnt1 = 0, cnt2 = 0;

    /* cell 0 */
    if (up   != -1 && rows[up][0]   == mv0) cnt0++;
    if (down != -1 && rows[down][0] == mv0) cnt0++;
    if (c1 == mv0) cnt0++;   /* right neighbor */

    /* cell 1 */
    if (up   != -1 && rows[up][1]   == mv1) cnt1++;
    if (down != -1 && rows[down][1] == mv1) cnt1++;
    if (c0 == mv1) cnt1++;   /* left neighbor */
    if (c2 == mv1) cnt1++;   /* right neighbor */

    /* cell 2 */
    if (up   != -1 && rows[up][2]   == mv2) cnt2++;
    if (down != -1 && rows[down][2] == mv2) cnt2++;
    if (c1 == mv2) cnt2++;   /* left neighbor */

    return (cnt0 == c0) && (cnt1 == c1) && (cnt2 == c2);
}

/* ------------------------------------------------------------------ */
/* Tables for dynamic programming, built once on first call          */

static bool initialized = false;

static int *adjacency_vals;          /* flattened adjacency lists */
static size_t adjacency_off[PAIRS+1];/* start indices of each pair's list */
static bool final_valid[PAIRS];      /* whether the bottom row of a pair is valid (down absent) */

static void init_tables(void)
{
    /* build all rows from 0..4 in base‑5 */
    for (int i = 0; i < ROWS; ++i) {
        int v = i;
        rows[i][0] = v % 5;
        rows[i][1] = (v / 5) % 5;
        rows[i][2] = (v / 25) % 5;
    }

    /* first count adjacency list sizes */
    int paircnt[PAIRS];
    memset(paircnt, 0, sizeof(paircnt));

    for (int i = 0; i < ROWS; ++i) {           /* up */
        for (int j = 0; j < ROWS; ++j) {       /* cur */
            int pid = i*ROWS + j;
            for (int k = 0; k < ROWS; ++k) {   /* next */
                if (is_valid_row(j, i, k))
                    paircnt[pid]++;
            }
        }
    }

    /* build offsets */
    adjacency_off[0] = 0;
    for (int i = 0; i < PAIRS; ++i)
        adjacency_off[i+1] = adjacency_off[i] + paircnt[i];

    adjacency_vals = (int *)malloc(adjacency_off[PAIRS] * sizeof(int));

    /* temporary counter for fill position of each list */
    int used[PAIRS];
    memset(used, 0, sizeof(used));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int pid = i*ROWS + j;
            int base = (int)adjacency_off[pid];
            for (int k = 0; k < ROWS; ++k) {
                if (is_valid_row(j, i, k))
                    adjacency_vals[base + used[pid]++] = k;
            }
        }
    }

    /* pre‑compute final_valid for each pair (up,cur) */
    for (int i = 0; i < PAIRS; ++i) {
        int up   = i / ROWS;
        int cur  = i % ROWS;
        final_valid[i] = is_valid_row(cur, up, -1);
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* DP arrays – static to avoid reallocating on every call            */

static seq_t dp_arr1[PAIRS];
static seq_t dp_arr2[PAIRS];

/* ------------------------------------------------------------------ */
/* Main function requested by the problem statement                  */

seq_t a(idx_t n)
{
    if (!initialized)
        init_tables();

    if (n == 0)          /* undefined for offset 1, but return 0 as safe default */
        return (seq_t)0;

    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int r = 0; r < ROWS; ++r) {
            if (is_valid_row(r, -1, -1))
                ans += (seq_t)1;
        }
        return ans;
    }

    seq_t *curr_dp = dp_arr1;
    seq_t *next_dp = dp_arr2;

    /* clear dp arrays */
    for (int i = 0; i < PAIRS; ++i) curr_dp[i] = (seq_t)0;
    for (int i = 0; i < PAIRS; ++i) next_dp[i] = (seq_t)0;

    /* initial step: first two rows */
    for (int r0 = 0; r0 < ROWS; ++r0)
        for (int r1 = 0; r1 < ROWS; ++r1)
            if (is_valid_row(r0, -1, r1))
                curr_dp[r0*ROWS + r1] = (seq_t)1;

    /* DP for rows 3 … n */
    for (idx_t step = 2; step < n; ++step) {
        for (int i = 0; i < PAIRS; ++i)
            next_dp[i] = (seq_t)0;

        for (int pid = 0; pid < PAIRS; ++pid) {
            seq_t val = curr_dp[pid];
            if (val == (seq_t)0)
                continue;
            size_t base = adjacency_off[pid];
            size_t end  = adjacency_off[pid+1];
            for (size_t idx = base; idx < end; ++idx) {
                int nxt = adjacency_vals[idx];
                int nid = (pid % ROWS) * ROWS + nxt;
                next_dp[nid] += val;
            }
        }
        /* swap buffers */
        seq_t *tmp = curr_dp;
        curr_dp = next_dp;
        next_dp = tmp;
    }

    /* final validation of the bottom row */
    seq_t ans = (seq_t)0;
    for (int pid = 0; pid < PAIRS; ++pid) {
        seq_t val = curr_dp[pid];
        if (val != (seq_t)0 && final_valid[pid])
            ans += val;
    }

    return ans;
}
