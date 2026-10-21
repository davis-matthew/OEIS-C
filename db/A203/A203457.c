/*
 * A203457  Number of nX5 0..2 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A203457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×5 arrays with entries 0..2 such that
 *            every nonzero entry is less than or equal to at least two of its
 *            horizontal and vertical neighbors
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   is_zero_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses a transfer–matrix dynamic programming.
   Each column of the array is encoded as a 5‑digit base‑3 number.
   For a column state we additionally keep a 5‑bit mask indicating
   which (nonzero) cells still need a right neighbor that is >=
   their own value.  The state space size is 3^5 × 2^5 = 7776. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_zero_seq.h"

/* Constants */
#define NUM_ROWS   5
#define NUM_COLS   243          /* 3^5 */
#define NUM_STATES (NUM_COLS * 32)   /* 2^NUM_ROWS */
#define MAX_TRANS  NUM_COLS       /* each state can transition to at most 243 columns */

/* Global tables (built once) */
static uint8_t  col_vals[NUM_COLS][NUM_ROWS];   /* value of each cell in a column */
static int      init_mask[NUM_COLS];             /* initial mask for a column (no left neighbor) */
static uint16_t trans_to[NUM_STATES * MAX_TRANS]; /* destination state indices for each source state */
static uint16_t trans_len[NUM_STATES];           /* number of outgoing transitions for each source state */

/* DP arrays (allocated once) */
static seq_t *dp_prev = NULL;   /* counts after the current number of processed columns */
static seq_t *dp_next = NULL;   /* scratch array */

/* Cache of sequence values */
static seq_t *val_cache = NULL;     /* val_cache[1..max_computed] holds a(n) */
static idx_t  max_computed = 0;     /* largest n for which a(n) is known */

/* Auxiliary tables */
static uint32_t pow3[NUM_ROWS + 1] = {0};
static bool initialized = false;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Build pow3[0..NUM_ROWS] */
static void build_pow3(void) {
    pow3[0] = 1;
    for (int i = 1; i <= NUM_ROWS; ++i) {
        pow3[i] = pow3[i-1] * 3U;
    }
}

/* ------------------------------------------------------------------ */
/* Generate all column configurations */
static void build_columns(void) {
    for (int idx = 0; idx < NUM_COLS; ++idx) {
        int tmp = idx;
        for (int r = 0; r < NUM_ROWS; ++r) {
            col_vals[idx][r] = tmp % 3;
            tmp /= 3;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compute the initial mask for a single column (no left neighbor) */
static int compute_init_mask(int col) {
    int mask = 0;
    for (int r = 0; r < NUM_ROWS; ++r) {
        int v = col_vals[col][r];
        if (v == 0) continue;          /* no constraint for zero cells */

        int s = 0;                     /* number of vertical neighbors >= v */
        if (r > 0 && col_vals[col][r-1] >= v) ++s;
        if (r+1 < NUM_ROWS && col_vals[col][r+1] >= v) ++s;

        if (s >= 2) { /* requirement already satisfied */
            continue;
        } else if (s == 1) {            /* needs right neighbor */
            mask |= (1 << r);
        } else {                        /* s == 0 => impossible */
            return -1;
        }
    }
    return mask;
}

/* ------------------------------------------------------------------ */
/* Compute transition mask from (prevCol, prevMask) and curCol.
   Return newMask or -1 if transition is invalid. */
static int compute_next_mask(int prevCol, int prevMask, int curCol) {
    int newMask = 0;
    for (int r = 0; r < NUM_ROWS; ++r) {
        int pv = col_vals[prevCol][r];
        int cv = col_vals[curCol][r];

        /* check pending requirement of previous cell */
        if (prevMask & (1 << r)) {
            /* previous cell still needs right neighbor >= its value */
            if (cv < pv) return -1;    /* not satisfied */
        }

        /* compute satisfied count for current cell */
        if (cv == 0) continue;          /* zero cells have no requirement */

        int s = 0;
        /* left neighbor */
        if (pv >= cv) ++s;
        /* up neighbor */
        if (r > 0 && col_vals[curCol][r-1] >= cv) ++s;
        /* down neighbor */
        if (r+1 < NUM_ROWS && col_vals[curCol][r+1] >= cv) ++s;

        if (s >= 2) { /* requirement already satisfied */
            continue;
        } else if (s == 1) {            /* needs right neighbor */
            newMask |= (1 << r);
        } else {                        /* s == 0 => impossible */
            return -1;
        }
    }
    return newMask;
}

/* ------------------------------------------------------------------ */
/* Populate transition table */
static void build_transitions(void) {
    for (int src = 0; src < NUM_STATES; ++src) {
        int prevCol  = src / 32;
        int prevMask = src % 32;
        uint16_t *dest = trans_to + src * MAX_TRANS;
        uint16_t cnt = 0;
        for (int curCol = 0; curCol < NUM_COLS; ++curCol) {
            int nMask = compute_next_mask(prevCol, prevMask, curCol);
            if (nMask >= 0) {
                dest[cnt++] = (uint16_t)(curCol * 32 + nMask);
            }
        }
        trans_len[src] = cnt;
    }
}

/* ------------------------------------------------------------------ */
/* Initialize all global data */
static void init_global(void) {
    if (initialized) return;
    build_pow3();
    build_columns();

    /* initial masks */
    for (int c = 0; c < NUM_COLS; ++c) {
        init_mask[c] = compute_init_mask(c);
    }

    build_transitions();

    /* allocate DP arrays */
    dp_prev = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));
    dp_next = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));

    /* allocate value cache (index 0 unused) */
    val_cache = (seq_t *)calloc(2, sizeof(seq_t));
    max_computed = 0;

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Ensure value cache can hold up to n */
static void ensure_cache(idx_t n) {
    if (n <= max_computed) return;
    idx_t new_cap = n + 1;                 /* indices 0..n */
    seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (val_cache) {
        memcpy(new_arr, val_cache, max_computed * sizeof(seq_t));
        free(val_cache);
    }
    val_cache = new_arr;
    max_computed = n;
}

/* ------------------------------------------------------------------ */
/* Count number of n×5 arrays satisfying the constraint.  */
seq_t a(idx_t n) {
    init_global();

    if (n == 0) return 0;                 /* not defined for offset<1 */

    /* Handle the case when we already know the value. */
    if (n <= max_computed) {
        return val_cache[n];
    }

    /* If this is the first call, compute results up to n. */
    /* dp_prev currently holds counts for cur_len columns already processed. */
    /* cur_len is implicit in max_computed: after each computed a(m) we store it. */
    /* We'll use a local variable cur_len initialized from max_computed. */
    static idx_t cur_len = 0;
    if (cur_len == 0) {        /* no columns processed yet */
        /* Initialize for first column */
        memset(dp_prev, 0, NUM_STATES * sizeof(seq_t));
        for (int col = 0; col < NUM_COLS; ++col) {
            int m = init_mask[col];
            if (m >= 0) {
                int st = col * 32 + m;
                dp_prev[st] = 1;
            }
        }
        cur_len = 1;
        /* compute a(1) */
        seq_t ans = 0;
        for (int col = 0; col < NUM_COLS; ++col) {
            int st = col * 32 + 0;           /* mask 0 */
            ans += dp_prev[st];
        }
        ensure_cache(1);
        val_cache[1] = ans;
    }

    /* Incrementally extend DP to the requested n */
    while (cur_len < n) {
        /* zero dp_next */
        memset(dp_next, 0, NUM_STATES * sizeof(seq_t));

        /* transition */
        for (int src = 0; src < NUM_STATES; ++src) {
            const seq_t cur_val = dp_prev[src];
            if (is_zero_seq(cur_val)) continue;
            uint16_t *dests = trans_to + src * MAX_TRANS;
            uint16_t cnt = trans_len[src];
            for (uint16_t i = 0; i < cnt; ++i) {
                int dst = dests[i];
                dp_next[dst] += cur_val;
            }
        }

        /* swap dp_prev and dp_next */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;

        ++cur_len;

        /* compute a(cur_len) */
        seq_t ans = 0;
        for (int col = 0; col < NUM_COLS; ++col) {
            int st = col * 32 + 0;           /* only states with mask 0 are valid */
            ans += dp_prev[st];
        }
        ensure_cache(cur_len);
        val_cache[cur_len] = ans;
    }

    return val_cache[n];
}
