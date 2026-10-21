/*
 * A234056  Number of (2+2)X(n+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally, diagonally downwards or antidiagonally
 *          downwards.
 * https://oeis.org/A234056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A234056:
 *  Number of 4×(n+2) arrays with entries in {0,1,2} that contain no
 *  increasing triple of length 3 horizontally, diagonally downwards
 *  or antidiagonally downwards.
 *
 *  The computation is performed by a transfer‑matrix dynamic programming
 *  over columns.  A state is a pair of consecutive columns, each
 *  consisting of 4 entries in base 3.  There are 3^8=6561 possible states.
 *  A transition from state (c1,c2) to (c2,c3) is allowed iff none of the
 *  eight 3‑term triples involving columns c1,c2,c3 is strictly increasing.
 *  The number of valid arrays of width (n+2) equals the sum of the state
 *  counts after n transitions (starting from all 6561 initial pairs).
 *
 *  The function memoises results to allow calls in any order.  It uses
 *  only integer operations that are guaranteed to work for all supported
 *  seq_t sizes.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Helper: decode a column (4 entries in base‑3) into an array of 4 ints.   */
static inline void decode_col(int col, int out[4]) {
    out[0] = col % 3;   col /= 3;
    out[1] = col % 3;   col /= 3;
    out[2] = col % 3;   col /= 3;
    out[3] = col;       /* last digit, 0..2 */
}

/* Helper: test whether adding col3 after (col1,col2) violates a rule.    */
static inline bool is_transition_ok(int col1, int col2, int col3) {
    int v1[4], v2[4], v3[4];
    decode_col(col1, v1);
    decode_col(col2, v2);
    decode_col(col3, v3);

    /* horizontal triples (same row) */
    for (int r = 0; r < 4; ++r)
        if (v1[r] < v2[r] && v2[r] < v3[r]) return false;

    /* diag downwards (slope +1) – rows 0‑2 and 1‑3 */
    for (int r = 0; r < 2; ++r)
        if (v1[r]     < v2[r+1] && v2[r+1] < v3[r+2]) return false;

    /* antidiagonal downwards (slope -1) – rows 2‑0 and 3‑1   */
    for (int r = 0; r < 2; ++r)
        if (v3[r]     < v2[r+1] && v2[r+1] < v1[r+2]) return false;

    return true;
}

/* --------------------------------------------------------------------- */
/* Build transition table once.  It is sparse, kept as a contiguous list.  */
static void build_transitions(void);
static size_t *edge_start   = NULL;  /* prefix sums, size MAX_STATE+1   */
static int    *edge_list    = NULL;  /* all target states, flat         */
static const size_t MAX_STATE = 6561;/* 3^8 states                         */
static bool built = false;

static void build_transitions(void) {
    size_t *out_deg = (size_t *)malloc(MAX_STATE * sizeof(size_t));
    if (!out_deg) exit(EXIT_FAILURE);

    /* Count out‑degrees */
    for (int id = 0; id < (int)MAX_STATE; ++id) {
        int c1 = id / 81;
        int c2 = id % 81;
        size_t deg = 0;
        for (int c3 = 0; c3 < 81; ++c3)
            if (is_transition_ok(c1, c2, c3)) ++deg;
        out_deg[id] = deg;
    }

    /* Prefix sums for offsets */
    edge_start = (size_t *)malloc((MAX_STATE + 1) * sizeof(size_t));
    if (!edge_start) exit(EXIT_FAILURE);
    edge_start[0] = 0;
    for (int i = 0; i < (int)MAX_STATE; ++i)
        edge_start[i + 1] = edge_start[i] + out_deg[i];

    size_t total_edges = edge_start[MAX_STATE];
    edge_list = (int *)malloc(total_edges * sizeof(int));
    if (!edge_list) exit(EXIT_FAILURE);

    /* Populate adjacency list */
    for (int id = 0; id < (int)MAX_STATE; ++id) {
        int c1 = id / 81;
        int c2 = id % 81;
        size_t pos = edge_start[id];
        for (int c3 = 0; c3 < 81; ++c3) {
            if (is_transition_ok(c1, c2, c3))
                edge_list[pos++] = c2 * 81 + c3;   /* target state */
        }
    }
    free(out_deg);
    built = true;
}

/* --------------------------------------------------------------------- */
/* DP vectors and memoisation                                            */
static seq_t *curr_dp = NULL;  /* after k steps, k columns added          */
static seq_t *next_dp = NULL;
static size_t curr_n  = 0;     /* largest n already computed              */
static seq_t *a_values  = NULL;
static size_t a_vals_cap = 0;

/* Ensure enough space for a_values array */
static void ensure_a_values_cap(size_t needed) {
    if (a_vals_cap >= needed) return;
    size_t new_cap = a_vals_cap ? a_vals_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(a_values, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    a_values = new_arr;
    a_vals_cap = new_cap;
}

/* Initialise DP vectors once.  All pairs of columns are initially allowed. */
static void init_dp_vectors(void) {
    curr_dp = (seq_t *)malloc(MAX_STATE * sizeof(seq_t));
    next_dp = (seq_t *)malloc(MAX_STATE * sizeof(seq_t));
    if (!curr_dp || !next_dp) exit(EXIT_FAILURE);
    for (size_t i = 0; i < MAX_STATE; ++i) curr_dp[i] = 1; /* two columns */
    /* a(1) will be computed after first transition below */
}

/* --------------------------------------------------------------------- */
/* Main sequence function */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* out of offset range, undefined */

    if (!built) build_transitions();
    if (!curr_dp) init_dp_vectors();

    /* Ensure memoisation array is large enough */
    ensure_a_values_cap((size_t)n + 1);

    /* Compute up to requested n if necessary */
    while (curr_n < n) {
        /* zero next vector */
        for (size_t i = 0; i < MAX_STATE; ++i) next_dp[i] = 0;

        /* transition step */
        for (size_t state = 0; state < MAX_STATE; ++state) {
            seq_t cnt = curr_dp[state];
            if (!cnt) continue;
            size_t start = edge_start[state];
            size_t end   = edge_start[state + 1];
            for (size_t p = start; p < end; ++p) {
                int target = edge_list[p];
                next_dp[target] += cnt;
            }
        }

        /* sum all counts to obtain a(curr_n+1) */
        seq_t total = 0;
        for (size_t i = 0; i < MAX_STATE; ++i) total += next_dp[i];
        a_values[++curr_n] = total;

        /* swap dp vectors */
        seq_t *tmp = curr_dp;
        curr_dp = next_dp;
        next_dp = tmp;
    }

    return a_values[n];
}
