/*
 * A202885  Number of nX4 0..2 arrays with every nonzero element less than or
 *          equal to some horizontal or vertical neighbor.
 * https://oeis.org/A202885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows of a 4‑column 0..2 array.
 * Counts all n×4 arrays in which every non‑zero element has a
 * horizontal or vertical neighbour with value ≥ it.
 * The DP is memoised: results for all 1≤k≤n are cached, so any
 * order of calls is supported.  The algorithm uses O(n·6561) time
 * and O(6561) memory, well under the limits for n≈10000.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compile‑time constants */
#define ROWS   81      /* 3^4 possible rows */
#define COLS   4
#define STATE  (ROWS * ROWS)

/* Working tables: values of each row in base‑3, horizontal masks,
 * and transition lists.  All are static so they are built once. */
static int row_val[ROWS][COLS];
static uint8_t horiz_mask[ROWS];
static int trans_next[STATE][ROWS];
static unsigned char trans_cnt[STATE];

/* Helper: fill a[row] with the digits of the base‑3 representation
 * of the integer r (0..80).  The least significant digit is at index 0. */
static void row_values(int r, int vec[COLS]) {
    int x = r;
    for (int i = 0; i < COLS; ++i) {
        vec[i] = x % 3;
        x /= 3;
    }
}

/* Pre‑compute row data and horizontal neighbour masks. */
static void precompute_rows(void) {
    for (int r = 0; r < ROWS; ++r) {
        row_values(r, row_val[r]);
        uint8_t mask = 0;
        for (int c = 0; c < COLS; ++c) {
            int v = row_val[r][c];
            if (v == 0) continue;
            int left  = (c > 0)   ? row_val[r][c-1] : -1;
            int right = (c < COLS-1) ? row_val[r][c+1] : -1;
            if (left >= v || right >= v) mask |= (1u << c);
        }
        horiz_mask[r] = mask;
    }
}

/* Test whether the middle row 'prev' is satisfied when surrounded
 * by 'prevprev' (up neighbour) and 'next' (down neighbour). */
static bool prev_row_satisfied(int prevprev, int prev, int next) {
    const int *pp = row_val[prevprev];
    const int *p  = row_val[prev];
    const int *n  = row_val[next];
    for (int c = 0; c < COLS; ++c) {
        int v = p[c];
        if (v == 0) continue;
        if (horiz_mask[prev] & (1u << c)) continue;
        if (pp[c] >= v) continue;
        if (n[c]  >= v) continue;
        return false;
    }
    return true;
}

/* Test whether the bottommost row 'last' is satisfied when the only
 * neighbour above it is 'prev'. */
static bool last_row_satisfied(int prev, int last) {
    const int *p = row_val[prev];
    const int *l = row_val[last];
    for (int c = 0; c < COLS; ++c) {
        int v = l[c];
        if (v == 0) continue;
        if (horiz_mask[last] & (1u << c)) continue;
        if (p[c] >= v) continue;
        return false;
    }
    return true;
}

/* Build the transition lists: for each pair (prevprev,prev) store all
 * next rows that make 'prev' satisfied.  The array trans_next holds
 * the next rows; trans_cnt gives how many each pair has. */
static void precompute_transitions(void) {
    for (int pp = 0; pp < ROWS; ++pp) {
        for (int p = 0; p < ROWS; ++p) {
            int idx = pp * ROWS + p;
            trans_cnt[idx] = 0;
            for (int n = 0; n < ROWS; ++n) {
                if (prev_row_satisfied(pp, p, n)) {
                    trans_next[idx][trans_cnt[idx]++] = n;
                }
            }
        }
    }
}

/* Static cache of already computed a(k) values.  count cache_len is
 * the number of filled elements.  cache_cap is the allocated size. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* DP state arrays: dp holds counts after the current row,
 * dp2 is the buffer for the next row.  active lists hold the
 * indices (prevprev*ROWS + prev) of states with non‑zero counts. */
static seq_t *dp  = NULL;
static seq_t *dp2 = NULL;
static int  *active = NULL;
static int  *next_active = NULL;
static size_t active_sz = 0;
static size_t active_cap = 0;

/* One time initialisation of all tables and arrays. */
static void init_once(void) {
    precompute_rows();
    precompute_transitions();
    dp = (seq_t*)malloc(STATE * sizeof(seq_t));
    dp2 = (seq_t*)malloc(STATE * sizeof(seq_t));
    active_cap = STATE;
    active = (int*)malloc(active_cap * sizeof(int));
    next_active = (int*)malloc(active_cap * sizeof(int));
}

/* Compute a(k) for all 1 ≤ k ≤ target.  If target ≤ cache_len, nothing
 * is done.  Otherwise the DP is continued from the last computed row. */
static void compute_up_to(idx_t target) {
    if (target <= cache_len) return;          /* already cached */
    static bool initialized = false;
    if (!initialized) { init_once(); initialized = true; }

    /* Grow cache if needed. */
    if (cache_cap < (size_t)target) {
        size_t new_cap = (cache_cap < 1024) ? 1024 : cache_cap * 2;
        if (new_cap < (size_t)target) new_cap = (size_t)target;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* If we are starting (no rows computed yet), initialise state
       for row 1. */
    if (cache_len == 0) {
        for (size_t i = 0; i < STATE; ++i) dp[i] = dp2[i] = 0;
        active_sz = 0;
        int zero = 0;
        for (int r = 0; r < ROWS; ++r) {
            int idx = zero * ROWS + r;
            dp[idx] = 1;
            active[active_sz++] = idx;
        }
        /* Count a(1) */
        seq_t sum = 0;
        for (size_t i = 0; i < active_sz; ++i) {
            int idx = active[i];
            int prev = idx / ROWS;
            int curr = idx % ROWS;
            if (last_row_satisfied(prev, curr)) sum += dp[idx];
        }
        cache[0] = sum;
        cache_len = 1;
    }

    /* Continue DP for rows cache_len+1 .. target. */
    for (idx_t r = (idx_t)cache_len + 1; r <= target; ++r) {
        /* Prepare dp2 */
        for (size_t i = 0; i < STATE; ++i) dp2[i] = 0;
        size_t next_sz = 0;

        for (size_t i = 0; i < active_sz; ++i) {
            int idx = active[i];
            seq_t val = dp[idx];
            if (!val) continue;
            int pp = idx / ROWS;
            int p  = idx % ROWS;
            int base = p * ROWS;
            unsigned char cnt = trans_cnt[pp * ROWS + p];
            for (unsigned char k = 0; k < cnt; ++k) {
                int next = trans_next[pp * ROWS + p][k];
                int new_idx = base + next;
                if (!dp2[new_idx]) {
                    next_active[next_sz++] = new_idx;
                }
                dp2[new_idx] += val;
            }
        }

        /* Swap buffers */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;

        /* Move next_active to active */
        for (size_t i = 0; i < next_sz; ++i) active[i] = next_active[i];
        active_sz = next_sz;

        /* Compute a(r) */
        seq_t sum = 0;
        for (size_t i = 0; i < active_sz; ++i) {
            int idx = active[i];
            int prev = idx / ROWS;
            int curr = idx % ROWS;
            if (last_row_satisfied(prev, curr)) sum += dp[idx];
        }
        cache[r-1] = sum;
    }
    cache_len = target;
}

/* ----------------------------------------------------------------- */
/* Public function required by the problem statement.                */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1, behaviour for 0 is arbitrary */
    compute_up_to(n);
    return cache[n-1];
}
