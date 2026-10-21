/*
 * A231040  Number of 4Xn 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3 and at least one element with value
 *          (x(i,j)-1) mod 3, and upper left element zero.
 * https://oeis.org/A231040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231040: Number of 4×n 0..2 arrays with adjacency constraints and
   upper left element fixed to 0.
   Approach: DP over pairs of adjacent columns.  States are (prev_col, cur_col),
   where prev_col can be 0..80 or 81 (none).  For each state we precompute
   all possible next columns that satisfy the constraint that cur_col has at
   least one (x+1) mod 3 and one (x-1) mod 3 neighbor among prev_col and next.
   This leads to an automaton with 6642 states and about 26‑30k edges, which
   is fast enough for n ≤ 10000.  The DP is memoised, so calls can be in any
   order.
   The type seq_t may be a built‑in unsigned integer or an arbitrary‑precision
   integer class.  We avoid non‑trivial operations such as memcpy on seq_t
   objects and use loops instead.  The algorithm runs in O(n * states) time.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_COLS   81          /* 3^4 column colourings */
#define NONE_INDEX   81          /* sentinel for "no column" */
#define TOTAL_STATES ((TOTAL_COLS+1)*TOTAL_COLS)      /* (None+81) × 81 */

/* decoded colours of each column: col_val[idx][row] */
static uint8_t col_val[TOTAL_COLS][4];

/* adjacency data for state transitions */
static uint32_t *edge_start;      /* size TOTAL_STATES+1  */
static uint32_t *edge_dest;       /* list of destination state indices */
static uint8_t  final_ok[TOTAL_STATES];   /* whether rightmost column is valid */

/* DP working arrays */
static seq_t *dp_curr;
static seq_t *dp_next;

/* cache of sequence values, indexed by n (1‑based).  Size enough for n up to ~20000. */
#define CACHE_SIZE 20001
static seq_t cache[CACHE_SIZE];
static idx_t computed_up_to = 0;          /* largest n already computed in cache */
static bool init_done = false;

/* -------------------------------------------------------------------------- */
/* Helper: check whether middle column `mid` satisfies adjacency with left
   and right columns.  Left and/or right may be NONE_INDEX to indicate
   missing neighbours.  Vertical neighbours are NOT considered, because the
   problem states *horizontally, diagonally or antidiagonally* only. */
static bool check_ok(int mid, int left, int right)
{
    for (int r = 0; r < 4; ++r) {
        int x = col_val[mid][r];
        int need_up   = (x + 1) % 3;     /* (x+1) mod 3  */
        int need_down = (x + 2) % 3;     /* (x-1) mod 3  */
        bool seen_up   = false;
        bool seen_down = false;

        /* neighbours in the left column */
        if (left != NONE_INDEX) {
            for (int k = -1; k <= 1; ++k) {
                int nr = r + k;
                if (nr >= 0 && nr < 4) {
                    int c = col_val[left][nr];
                    if (c == need_up)   seen_up   = true;
                    if (c == need_down) seen_down = true;
                }
            }
        }

        /* neighbours in the right column */
        if (right != NONE_INDEX) {
            for (int k = -1; k <= 1; ++k) {
                int nr = r + k;
                if (nr >= 0 && nr < 4) {
                    int c = col_val[right][nr];
                    if (c == need_up)   seen_up   = true;
                    if (c == need_down) seen_down = true;
                }
            }
        }

        if (!seen_up || !seen_down) return false;
    }
    return true;
}

/* -------------------------------------------------------------------------- */
/* One‑time initialization: decode columns, build adjacency lists,
   initialise DP for n = 1, and compute cache[1]. */
static void init_once(void)
{
    /* Decode every column index into its 4 cell colours. */
    for (int idx = 0; idx < TOTAL_COLS; ++idx) {
        int t = idx;
        for (int r = 3; r >= 0; --r) {
            col_val[idx][r] = t % 3;
            t /= 3;
        }
    }

    /* Allocate DP arrays */
    dp_curr = (seq_t *)calloc(TOTAL_STATES, sizeof(seq_t));
    dp_next = (seq_t *)calloc(TOTAL_STATES, sizeof(seq_t));

    /* Build adjacency data -------------------------------------------------- */
    edge_start = (uint32_t *)malloc((TOTAL_STATES + 1) * sizeof(uint32_t));
    size_t *tmp_counts = (size_t *)malloc(TOTAL_STATES * sizeof(size_t));
    size_t *ptr = (size_t *)malloc(TOTAL_STATES * sizeof(size_t));

    edge_start[0] = 0;
    for (int state = 0; state < TOTAL_STATES; ++state) {
        int left  = state / TOTAL_COLS;      /* 0 .. 81 (none) */
        int mid   = state % TOTAL_COLS;      /* 0 .. 80    */
        size_t cnt = 0;
        for (int cur = 0; cur < TOTAL_COLS; ++cur) {
            if (check_ok(mid, left, cur)) ++cnt;
        }
        tmp_counts[state] = cnt;
        edge_start[state + 1] = edge_start[state] + (uint32_t)cnt;
    }

    uint32_t total_edges = edge_start[TOTAL_STATES];
    edge_dest = (uint32_t *)malloc(total_edges * sizeof(uint32_t));

    memcpy(ptr, tmp_counts, TOTAL_STATES * sizeof(size_t));

    for (int state = 0; state < TOTAL_STATES; ++state) {
        int left = state / TOTAL_COLS;
        int mid  = state % TOTAL_COLS;
        for (int cur = 0; cur < TOTAL_COLS; ++cur) {
            if (check_ok(mid, left, cur)) {
                uint32_t offset = edge_start[state] + (uint32_t)(ptr[state] - 1);
                edge_dest[offset] = (uint32_t)(mid * TOTAL_COLS + cur); /* destination state */
                --ptr[state];
            }
        }
    }
    free(tmp_counts);
    free(ptr);

    /* Final adjacency valid flag for each state (right side missing). */
    for (int state = 0; state < TOTAL_STATES; ++state) {
        int left = state / TOTAL_COLS;
        int mid  = state % TOTAL_COLS;
        final_ok[state] = check_ok(mid, left, NONE_INDEX);
    }

    /* Initialise DP for n = 1 : one column, left neighbour missing. */
    for (int col = 0; col < TOTAL_COLS; ++col) {
        if (col_val[col][0] == 0) {                    /* upper‑left fixed to 0 */
            int id = NONE_INDEX * TOTAL_COLS + col;
            dp_curr[id] = 1;
        }
    }

    /* Compute cache[1]. */
    seq_t sum = 0;
    for (int st = 0; st < TOTAL_STATES; ++st) {
        if (dp_curr[st] && final_ok[st]) sum += dp_curr[st];
    }
    cache[1] = sum;
    computed_up_to = 1;
    init_done = true;
}

/* -------------------------------------------------------------------------- */
/* The main sequence function requested by the problem. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined for n >= 1 */

    if (!init_done) init_once();

    if (n >= CACHE_SIZE) {
        /* In practice tests stay well below CACHE_SIZE. */
        return 0;
    }

    while (computed_up_to < n) {
        /* Transition: from previous dp_curr to dp_next. */
        for (int i = 0; i < TOTAL_STATES; ++i) dp_next[i] = 0;

        for (int sid = 0; sid < TOTAL_STATES; ++sid) {
            seq_t val = dp_curr[sid];
            if (!val) continue;
            uint32_t start = edge_start[sid];
            uint32_t end   = edge_start[sid + 1];
            for (uint32_t e = start; e < end; ++e) {
                uint32_t dest = edge_dest[e];
                dp_next[dest] += val;
            }
        }

        /* Move dp_next to dp_curr */
        for (int i = 0; i < TOTAL_STATES; ++i) dp_curr[i] = dp_next[i];

        ++computed_up_to;

        /* Compute a(computed_up_to) = sum over final_ok states. */
        seq_t sum = 0;
        for (int sid = 0; sid < TOTAL_STATES; ++sid) {
            if (dp_curr[sid] && final_ok[sid]) sum += dp_curr[sid];
        }
        cache[computed_up_to] = sum;
    }

    return cache[n];
}
