/*
 * A209221  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A209221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding horizontal 0 0 0 and
 *            0 1 0, and vertical 0 1 0 and 1 1 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 28, u64 n <= 62, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts the number of such arrays for any n ≥ 1,
   using dynamic programming on pairs of rows. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static bool initialized = false;

/* Cache of computed a(n) values, 1‑based. */
static seq_t *a_cache = NULL;     /* sequence values */
static idx_t  cache_size = 0;     /* last index stored */

/* Global data used by the DP. */
static int rows[32];          /* allowed row patterns (≤15) */
static int rowsCount = 0;     /* number of allowed rows */

/* Vertical transition table:
   trans[p][q][r] == 1 iff rows p, q, r (indices into rows[])
   satisfy the vertical restrictions. */
static uint8_t trans[32][32][32];

/* DP arrays for states of last two rows. */
static seq_t dp1[32][32];
static seq_t dp2[32][32];
static seq_t (*dpPrev)[32];   /* points to the array that holds states for current length */
static seq_t (*dpNext)[32];   /* points to the array used for the next length */

/* Current maximum n for which a_cache and dpPrev have been fully built. */
static idx_t current_len = 0;

/* ------------------------------------------------------------------ */
/* Helper: compute the set of allowed single‑row patterns.            */
static void compute_allowed_rows(void) {
    rowsCount = 0;
    for (int r = 0; r < 32; ++r) {
        bool ok = true;
        for (int c = 0; c < 3 && ok; ++c) {
            int sub = (r >> c) & 7;               /* three consecutive bits */
            if (sub == 0 || sub == 2)             /* 000 or 010 forbidden */
                ok = false;
        }
        if (ok)
            rows[rowsCount++] = r;
    }
}

/* ------------------------------------------------------------------ */
/* Helper: build the vertical transition table.                        */
static void compute_transitions(void) {
    for (int i = 0; i < rowsCount; ++i)
        for (int j = 0; j < rowsCount; ++j)
            for (int k = 0; k < rowsCount; ++k) {
                int pi = rows[i];
                int pj = rows[j];
                int pk = rows[k];
                bool ok = true;
                for (int col = 0; col < 5 && ok; ++col) {
                    int a = (pi >> col) & 1;
                    int b = (pj >> col) & 1;
                    int c = (pk >> col) & 1;
                    if ((a == 0 && b == 1 && c == 0) ||
                        (a == 1 && b == 1 && c == 1))
                        ok = false;
                }
                trans[i][j][k] = ok ? 1 : 0;
            }
}

/* ------------------------------------------------------------------ */
/* Initialization routine.                                            */
static void init(void) {
    compute_allowed_rows();
    compute_transitions();

    /* Allocate cache for a(1) and a(2). */
    cache_size = 2;
    a_cache = (seq_t *)malloc((cache_size + 2) * sizeof(seq_t));
    a_cache[0] = 0; /* unused */
    a_cache[1] = (seq_t)rowsCount;           /* a(1) */
    a_cache[2] = a_cache[1] * a_cache[1];    /* a(2) = N^2 */

    /* Initialise DP state for length 2: all pairs are allowed. */
    dpPrev = dp1;
    dpNext = dp2;
    for (int i = 0; i < rowsCount; ++i)
        for (int j = 0; j < rowsCount; ++j)
            dpPrev[i][j] = (seq_t)1;

    current_len = 2;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Extend precomputation up to n (inclusive) and return a(n).         */
seq_t a(idx_t n) {
    if (!initialized)
        init();

    if (n <= cache_size)
        return a_cache[n];

    /* Grow cache if necessary. */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(a_cache, (n + 2) * sizeof(seq_t));
        a_cache = new_cache;
        cache_size = n;
    }

    /* Extend DP from current_len+1 up to n. */
    for (idx_t step = current_len + 1; step <= n; ++step) {
        /* Clear dpNext. */
        for (int i = 0; i < rowsCount; ++i)
            for (int j = 0; j < rowsCount; ++j)
                dpNext[i][j] = 0;

        /* Transition. */
        for (int p = 0; p < rowsCount; ++p) {
            for (int q = 0; q < rowsCount; ++q) {
                seq_t ways = dpPrev[p][q];
                if (ways == 0)
                    continue;
                for (int r = 0; r < rowsCount; ++r) {
                    if (trans[p][q][r]) {
                        dpNext[q][r] += ways;
                    }
                }
            }
        }

        /* Sum all states to get a(step). */
        seq_t sum = 0;
        for (int p = 0; p < rowsCount; ++p)
            for (int q = 0; q < rowsCount; ++q)
                sum += dpNext[p][q];
        a_cache[step] = sum;

        /* Prepare for next step: swap dpPrev and dpNext. */
        seq_t (*tmp)[32] = dpPrev;
        dpPrev = dpNext;
        dpNext = tmp;
    }

    current_len = n;
    return a_cache[n];
}
