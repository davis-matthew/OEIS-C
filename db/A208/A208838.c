/*
 * A208838  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: DP for A208838 (OEIS)  
   Number of n×6 binary arrays avoiding 000/001 horizontally and 011/110 vertically.
   The DP keeps states for the last two rows (indices into the list of allowed
   6‑bit rows).  The transition masks are pre‑computed once.
   Complexity: O(n·R²) where R = number of allowed rows (here R = 26) */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* The types idx_t and seq_t are supplied by the build system; no redeclaration is required. */

/* ------------------------------------------------------------------ */
/* Precomputation: allowed row masks and transition masks            */
/* ------------------------------------------------------------------ */
#define MAX_ROWS 64               /* 6 bits → 64 possible row masks */

/* List of allowed 6‑bit rows and lookup table                     */
static int  rows[MAX_ROWS];      /* indices of allowed rows */
static int  row_index[MAX_ROWS]; /* -1 or index of row in rows[] */
static size_t num_rows;          /* number of allowed rows */

/* Transition masks: next_mask[p*num_rows + q] gives a bitmask of allowed
   next rows r (encoded as indices) after previous two rows p,q.
   Since next_mask is a bitmask of length <= num_rows, uint64_t is enough. */
static uint64_t *next_mask = NULL;
static size_t state_size;        /* num_rows * num_rows */

/* ------------------------------------------------------------------ */
/* DP tables for the last two computed lengths                       */
/* ------------------------------------------------------------------ */
static seq_t *dp_prev = NULL;    /* DP for length L-2 */
static seq_t *dp_curr = NULL;    /* DP for length L-1 */

/* ------------------------------------------------------------------ */
/* Cache of computed sequence values: a_cache[n] == a(n) (1‑based)  */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;    /* index 0 unused */
static size_t a_cap   = 0;        /* allocated size of a_cache */
static idx_t  last_len = 0;       /* largest n that has been computed */

/* ------------------------------------------------------------------ */
/* Determine whether a 6‑bit pattern avoids 000 and 001 horizontally */
/* ------------------------------------------------------------------ */
static int allowed_row(int r)
{
    for (int i = 0; i <= 3; ++i) {
        int b0 = (r >> i)       & 1;
        int b1 = (r >> (i + 1)) & 1;
        int b2 = (r >> (i + 2)) & 1;
        if (b0 == 0 && b1 == 0 && b2 == 0) return 0;  /* 000 */
        if (b0 == 0 && b1 == 0 && b2 == 1) return 0;  /* 001 */
    }
    return 1;
}

/* ------------------------------------------------------------------ */
/* Perform all pre‑computations needed for the DP.                     */
/* ------------------------------------------------------------------ */
static void init_precomputations(void)
{
    /* Collect all allowed 6‑bit rows. */
    num_rows = 0;
    for (int r = 0; r < 64; ++r) {
        if (allowed_row(r)) {
            rows[num_rows++] = r;
        }
    }

    /* Build row index table. */
    for (int i = 0; i < 64; ++i) row_index[i] = -1;
    for (size_t i = 0; i < num_rows; ++i) row_index[rows[i]] = (int)i;

    /* Allocate DP tables. */
    state_size = num_rows * num_rows;
    dp_prev = (seq_t *)malloc(state_size * sizeof(seq_t));
    dp_curr = (seq_t *)malloc(state_size * sizeof(seq_t));
    memset(dp_prev, 0, state_size * sizeof(seq_t));
    memset(dp_curr, 0, state_size * sizeof(seq_t));

    /* Allocate transition masks. */
    next_mask = (uint64_t *)malloc(state_size * sizeof(uint64_t));
    for (size_t p = 0; p < num_rows; ++p) {
        for (size_t q = 0; q < num_rows; ++q) {
            uint64_t mask = 0;
            for (size_t r = 0; r < num_rows; ++r) {
                int valid = 1;
                for (int col = 0; col < 6 && valid; ++col) {
                    int b1 = (rows[p] >> col) & 1;   /* row p */
                    int b2 = (rows[q] >> col) & 1;   /* row q */
                    int b3 = (rows[r] >> col) & 1;   /* candidate row r */
                    if (b1 == 0 && b2 == 1 && b3 == 1) valid = 0; /* 011 */
                    if (b1 == 1 && b2 == 1 && b3 == 0) valid = 0; /* 110 */
                }
                if (valid) mask |= ((uint64_t)1 << r);
            }
            next_mask[p * num_rows + q] = mask;
        }
    }

    /* Initialize base DP for length 2: all pairs allowed. */
    for (size_t idx = 0; idx < state_size; ++idx) dp_prev[idx] = (seq_t)1;
}

/* ------------------------------------------------------------------ */
/* Ensure that a_cache contains values up to index n.                  */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    /* First call: perform precomputation and initialize base cases. */
    if (a_cache == NULL) {
        init_precomputations();

        /* Allocate cache for indices 0..2 (index 0 unused). */
        a_cap = 3;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)0;            /* unused */
        a_cache[1] = (seq_t)num_rows;     /* a(1) */
        a_cache[2] = (seq_t)(num_rows * (seq_t)num_rows); /* a(2) */

        last_len = 2;                     /* we have computed up to n=2 */
    }

    /* Resize cache if needed. */
    if ((idx_t)(a_cap) <= n) {
        a_cap = (size_t)(n) + 1;
        a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
    }

    /* Incrementally compute missing terms. */
    for (idx_t len = last_len + 1; len <= n; ++len) {
        /* Zero dp_curr. */
        memset(dp_curr, 0, state_size * sizeof(seq_t));

        /* From dp_prev (length len-2) to dp_curr (length len-1). */
        for (size_t p = 0; p < num_rows; ++p) {
            for (size_t q = 0; q < num_rows; ++q) {
                seq_t val = dp_prev[p * num_rows + q];
                if (val == 0) continue;
                uint64_t mask = next_mask[p * num_rows + q];
                while (mask) {
                    unsigned r = (unsigned)__builtin_ctzll(mask);
                    mask &= mask - 1;
                    size_t idx = q * num_rows + r;
                    dp_curr[idx] += val;
                }
            }
        }

        /* Sum all dp_curr entries to obtain a(len). */
        seq_t sum = 0;
        for (size_t i = 0; i < state_size; ++i) sum += dp_curr[i];
        a_cache[len] = sum;

        /* Prepare for next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
        last_len = len;
    }
}

/* ------------------------------------------------------------------ */
/* Public function: value of sequence A208838 at position n.           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
