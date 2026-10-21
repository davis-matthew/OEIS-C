/*
 * A238041  Number of (n+1)X(3+1) 0..2 arrays with the upper median minus the
 *          lower median of every 2X2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A238041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
    OEIS A238041 – Number of (n+1)×4 arrays over {0,1,2} such that  
    for every 2×2 subblock the difference upper‑median – lower‑median
    of that block differs by exactly one from the neighboring blocks
    horizontally and vertically.
    The terms are obtained by a transfer‑matrix dynamic programming
    on pairs of consecutive rows.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build system. */

#define MAX_ROWS    81              /* 3³ = 27, 4 columns → 81 patterns */
#define MAX_STATE   (MAX_ROWS * MAX_ROWS)

/* representation of a row pattern (base‑3 encoding) */
static int row_vals[MAX_ROWS][4];

/* state {row_i , row_{i+1} , diff[3]} */
static int state_r1[MAX_STATE];
static int state_r2[MAX_STATE];
static int state_diff[MAX_STATE][3];
static int state_index[MAX_ROWS][MAX_ROWS];   /* -1 if no state */

/* for each row i: list of state indices whose first row is i */
static int state_start[MAX_ROWS][MAX_ROWS];
static int state_start_cnt[MAX_ROWS];

/* adjacency lists of states: state s → list of t */
static int *edge_start;      /* start position of edges of state s */
static int *edge_count;      /* number of outgoing edges of state s */
static int *edge_list;

/* total number of valid states */
static size_t Nstates = 0;
static bool initialized = false;

/* ---------------------------------------------------------- */
/* Utility: sort four integers in place (simple insertion sort) */
static void sort4(int a[4])
{
    for (int i = 1; i < 4; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

/* compute diff vector (3 ints) for pair of rows (i,j) */
static void pair_diff(int i, int j, int out[3])
{
    int vals[4];
    for (int k = 0; k < 3; ++k) {
        vals[0] = row_vals[i][k];
        vals[1] = row_vals[i][k + 1];
        vals[2] = row_vals[j][k];
        vals[3] = row_vals[j][k + 1];
        sort4(vals);
        out[k] = vals[2] - vals[1];
    }
}

/* check horizontal adjacency condition for a diff triple */
static inline bool horizontal_ok(const int diff[3])
{
    return (abs(diff[0] - diff[1]) == 1) && (abs(diff[1] - diff[2]) == 1);
}

/* vertical adjacency check between two states */
static inline bool vertical_ok(int s_idx, int t_idx)
{
    for (int k = 0; k < 3; ++k) {
        int d = state_diff[s_idx][k] - state_diff[t_idx][k];
        if (d != 1 && d != -1) return false;
    }
    return true;
}

/* ---------------------------------------------------------- */
/* build all helper data structures (runs once) */
static void init_structures(void)
{
    /* 1. fill row patterns */
    for (int id = 0; id < MAX_ROWS; ++id) {
        int tmp = id;
        for (int c = 0; c < 4; ++c) {
            row_vals[id][c] = tmp % 3;
            tmp /= 3;
        }
    }

    /* init state_index to -1 */
    for (int i = 0; i < MAX_ROWS; ++i)
        for (int j = 0; j < MAX_ROWS; ++j)
            state_index[i][j] = -1;

    /* 2. create valid states */
    int diff_triple[3];
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            pair_diff(i, j, diff_triple);
            if (!horizontal_ok(diff_triple))
                continue;
            int idx = (int)Nstates;
            state_r1[idx] = i;
            state_r2[idx] = j;
            memcpy(state_diff[idx], diff_triple, 3 * sizeof(int));
            state_index[i][j] = idx;
            state_start[i][state_start_cnt[i]++] = idx;
            ++Nstates;
        }
    }

    /* 3. first pass: compute edge counts and total edges */
    int *tmp_edge_count = (int *)malloc(Nstates * sizeof(int));
    size_t total_edges = 0;
    for (size_t s = 0; s < Nstates; ++s) {
        int r2 = state_r2[s];
        int cnt = 0;
        for (int k = 0; k < state_start_cnt[r2]; ++k) {
            int t = state_start[r2][k];
            if (vertical_ok((int)s, t))
                ++cnt;
        }
        tmp_edge_count[s] = cnt;
        total_edges += (size_t)cnt;
    }

    /* allocate adjacency structures */
    edge_start = (int *)malloc(Nstates * sizeof(int));
    edge_count = (int *)malloc(Nstates * sizeof(int));
    edge_list  = (int *)malloc(total_edges * sizeof(int));

    /* 4. second pass: fill edges */
    size_t pos = 0;
    for (size_t s = 0; s < Nstates; ++s) {
        edge_start[s] = (int)pos;
        int r2 = state_r2[s];
        int cnt = 0;
        for (int k = 0; k < state_start_cnt[r2]; ++k) {
            int t = state_start[r2][k];
            if (vertical_ok((int)s, t)) {
                edge_list[pos++] = t;
                ++cnt;
            }
        }
        edge_count[s] = cnt;
    }

    free(tmp_edge_count);
    initialized = true;
}

/* ---------------------------------------------------------- */
/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    if (!initialized) init_structures();

    /* dynamic programming over states */
    size_t S = Nstates;

#ifdef __cplusplus
    /* C++ path: use std::vector for automatic construction / destruction */
    #include <vector>
    std::vector<seq_t> dp(S, seq_t(1));
    std::vector<seq_t> ndp(S);
#else
    /* C path: use malloc; assume seq_t is an unsigned integer type */
    seq_t *dp  = (seq_t *)malloc(S * sizeof(seq_t));
    seq_t *ndp = (seq_t *)malloc(S * sizeof(seq_t));
    for (size_t i = 0; i < S; ++i) dp[i] = 1;          /* initial counts */
#endif

    /* perform n-1 transitions (for n rows we need n-1 edges in the state graph) */
    for (idx_t step = 1; step < n; ++step) {
#ifdef __cplusplus
        std::fill(ndp.begin(), ndp.end(), seq_t(0));
#else
        for (size_t i = 0; i < S; ++i) ndp[i] = 0;
#endif
        for (size_t s = 0; s < S; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int start = edge_start[s];
            int cnt   = edge_count[s];
            for (int e = 0; e < cnt; ++e) {
                int t = edge_list[start + e];
#ifdef __cplusplus
                ndp[t] += val;
#else
                ndp[t] += val;
#endif
            }
        }
#ifdef __cplusplus
        dp.swap(ndp);
#else
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
#endif
    }

    /* sum all ending states */
    seq_t result = 0;
    for (size_t i = 0; i < S; ++i) {
        result += dp[i];
    }

#ifndef __cplusplus
    free(dp);
    free(ndp);
#endif
    return result;
}
