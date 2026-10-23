/*
 * A196453  Number of n X 5 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,2,4,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A196453
 * Formula: This sequence is computed by dynamic programming on rows of
 * an n×5 array.  Each state consists of a row pattern (5 cells, each in 0..4)
 * and a 5‑bit mask indicating for each column whether the cell in the next
 * row must be equal to the target of the current cell.  Transitions enforce
 * horizontal constraints inside the row and vertical constraints with the
 * preceding row.  The number of arrays for a given n is the sum of
 * dp‑values of states having zero mask after processing all rows.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Target values for each symbol */
static const int T[5]  = {3, 0, 2, 4, 1};
static const int K[5]  = {0, 1, 2, 3, 4};          /* required neighbour count */

/* Maximum number of patterns: 5^5 = 3125, 5 bits for mask */
#define MAX_PAT    3125
#define MAX_MASK   32          /* 5 bits */
#define MAX_STATES (MAX_PAT * MAX_MASK)

/* Global data that are initialized once */
static int      pat_val[MAX_PAT][5];      /* pattern values */
static int      base_rem[MAX_PAT][5];     /* vertical neighbours needed after left/right */
static bool     pat_valid[MAX_PAT];       /* pattern valid for interior rows */
static int      top_mask[MAX_PAT];        /* bitmask of needed down neighbours for top row */
static bool     top_valid[MAX_PAT];       /* pattern valid as top row */

/* State mapping */
static int      state_id[MAX_PAT][MAX_MASK];
static int      state_pat[MAX_STATES];    /* pattern index for each state */
static int      state_mask[MAX_STATES];   /* mask for each state */

/* Adjacency representation */
static int      *edge_to     = NULL;      /* destination state */
static int      *edge_next   = NULL;      /* next edge index */
static int      *head        = NULL;      /* head of list for each state */
static int      edge_cnt;                 /* number of edges */

/* DP arrays */
static seq_t    *dp          = NULL;
static seq_t    *dp_next     = NULL;
static int      state_cnt    = 0;

/* Allocate and initialise all data (executed on first call) */
static void init(void) {
    int i, c;
    int pat_index = 0;
    int valid_count = 0;
    size_t cap_edges = 1 << 20;          /* start with 1M edges */
    /* Initialise pattern tables */
    for (pat_index = 0; pat_index < MAX_PAT; ++pat_index) {
        int x = pat_index;
        for (c = 0; c < 5; ++c) {
            pat_val[pat_index][c] = x % 5;
            x /= 5;
        }
        bool ok = true;
        for (c = 0; c < 5; ++c) {
            int v       = pat_val[pat_index][c];
            int left    = (c > 0) && (pat_val[pat_index][c-1] == T[v]) ? 1 : 0;
            int right   = (c < 4) && (pat_val[pat_index][c+1] == T[v]) ? 1 : 0;
            int rem     = K[v] - (left + right);
            base_rem[pat_index][c] = rem;
            if (rem < 0 || rem > 2) ok = false;      /* interior restrictions */
        }
        pat_valid[pat_index] = ok;
        if (ok) valid_count++;

        /* top row mask: only 0 or 1 needed */
        bool top_ok = true;
        int mask = 0;
        for (c = 0; c < 5; ++c) {
            int rem = base_rem[pat_index][c];
            if (rem < 0 || rem > 1) { top_ok = false; break; }
            if (rem == 1) mask |= 1 << c;
        }
        top_valid[pat_index] = top_ok;
        top_mask[pat_index]  = mask;
    }

    /* State mapping: all (pat,mask) pairs for valid patterns */
    state_cnt = valid_count * MAX_MASK;
    for (int pat = 0; pat < MAX_PAT; ++pat)
        for (int m = 0; m < MAX_MASK; ++m)
            state_id[pat][m] = -1;

    int sid = 0;
    for (int pat = 0; pat < MAX_PAT; ++pat) {
        if (!pat_valid[pat]) continue;
        for (int m = 0; m < MAX_MASK; ++m) {
            state_id[pat][m] = sid;
            state_pat[sid]   = pat;
            state_mask[sid]  = m;
            ++sid;
        }
    }

    /* Allocate adjacency storage */
    edge_to   = (int *)malloc(cap_edges * sizeof(int));
    edge_next = (int *)malloc(cap_edges * sizeof(int));
    head      = (int *)malloc(state_cnt * sizeof(int));
    memset(head, -1, state_cnt * sizeof(int));
    edge_cnt  = 0;

    /* Build transitions */
    for (int pat_from = 0; pat_from < MAX_PAT; ++pat_from) {
        if (!pat_valid[pat_from]) continue;
        for (int pat_to = 0; pat_to < MAX_PAT; ++pat_to) {
            if (!pat_valid[pat_to]) continue;
            int eq_mask = 0;
            int new_mask = 0;
            bool ok = true;
            for (c = 0; c < 5; ++c) {
                int vP = pat_val[pat_from][c];
                int vC = pat_val[pat_to][c];
                int tP = T[vP];
                int up_match = (vP == T[vC]) ? 1 : 0;
                if (vC == tP) eq_mask |= 1 << c;
                int rem_needed = base_rem[pat_to][c] - up_match;
                if (rem_needed < 0 || rem_needed > 1) { ok = false; break; }
                if (rem_needed == 1) new_mask |= 1 << c;
            }
            if (!ok) continue;
            int from_id = state_id[pat_from][eq_mask];
            int to_id   = state_id[pat_to][new_mask];
            /* If arrays full, expand */
            if (edge_cnt >= (int)cap_edges) {
                cap_edges <<= 1;
                edge_to   = (int *)realloc(edge_to,   cap_edges * sizeof(int));
                edge_next = (int *)realloc(edge_next, cap_edges * sizeof(int));
            }
            edge_next[edge_cnt] = head[from_id];
            edge_to[edge_cnt]   = to_id;
            head[from_id] = edge_cnt;
            ++edge_cnt;
        }
    }

    /* Allocate DP buffers */
    dp      = (seq_t *)malloc(state_cnt * sizeof(seq_t));
    dp_next = (seq_t *)malloc(state_cnt * sizeof(seq_t));
    memset(dp, 0, state_cnt * sizeof(seq_t));
    memset(dp_next, 0, state_cnt * sizeof(seq_t));
}

/* Main sequence function */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init();
        initialized = true;
    }

    /* Reset DP arrays */
    for (int i = 0; i < state_cnt; ++i) {
        dp[i] = 0;
        dp_next[i] = 0;
    }

    /* Initial row (top) */
    for (int pat = 0; pat < MAX_PAT; ++pat) {
        if (!top_valid[pat]) continue;
        int id = state_id[pat][top_mask[pat]];
        dp[id] = 1;
    }

    if (n == 1) {
        /* Sum states with zero mask */
        seq_t ans = 0;
        for (int s = 0; s < state_cnt; ++s)
            if (state_mask[s] == 0) ans = ans + dp[s];
        return ans;
    }

    /* Process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* Clear next buffer */
        for (int i = 0; i < state_cnt; ++i) dp_next[i] = 0;

        for (int s = 0; s < state_cnt; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int e = head[s];
            while (e != -1) {
                int to = edge_to[e];
                dp_next[to] = dp_next[to] + val;
                e = edge_next[e];
            }
        }
        /* Swap buffers */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* Final result: states with no pending down neighbour requirement */
    seq_t ans = 0;
    for (int s = 0; s < state_cnt; ++s)
        if (state_mask[s] == 0) ans = ans + dp[s];

    return ans;
}
