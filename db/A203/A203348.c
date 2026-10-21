/*
 * A203348  Number of (n+2)X3 binary arrays avoiding patterns 010 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A203348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   bad_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203348: number of (n+2)×3 binary arrays avoiding patterns 010
 * and 101 in rows, columns and NW–SE diagonals.
 *
 * The computation is performed by dynamic programming on pairs of
 * consecutive columns.  Each column is a 3‑bit pattern that itself must
 * avoid 010 and 101 (there are 6 valid patterns).  A state is an
 * ordered pair of the last two columns; a transition to a third
 * column is allowed iff all row and diagonal constraints are satisfied.
 * For a matrix with m columns the number of arrays is the number of
 * walks of length m−2 in this transition graph, starting from any
 * pair of columns.  The algorithm runs in O(n) time for each n
 * (n up to about 10 000 in the tests).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bad_pattern.h"

/* ---------- Constants and global storage ---------- */

#define STATE_COLS 6          /* number of allowed column patterns */
#define STATES (STATE_COLS * STATE_COLS)

/* mapping of allowed column patterns (bit representation)  */
static const int allowed[STATE_COLS] = {0, 1, 3, 4, 6, 7};

/* adjacency: toCount[idx] and toNext[idx][k] */
static int toCount[STATES];
static int toNext[STATES][STATE_COLS];

/* cache of computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of allocated slots in cache */
static idx_t computed = 0;     /* maximum n for which cache[·] is filled */

/* DP state for the current number of processed columns (after
 * 'computed' steps).  Each entry corresponds to a pair of last
 * two columns.  We keep both the current and the next temporary
 * state arrays. */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

/* ---------- Helper functions ---------- */

/* Test whether a transition from column pair (p2,p1) to new column p3
 * is allowed under all constraints.  All parameters are indices into
 * the `allowed` array. */
static inline bool transition_ok(int p2, int p1, int p3)
{
    int col2 = allowed[p2];
    int col1 = allowed[p1];
    int col3 = allowed[p3];

    /* check each row for the forbidden horizontal patterns */
    for (int r = 0; r < 3; ++r) {
        int a = (col2 >> r) & 1;
        int b = (col1 >> r) & 1;
        int c = (col3 >> r) & 1;
        if (bad_pattern(a, b, c))
            return false;
    }

    /* check the sole NW–SE diagonal of length 3 */
    int a = (col2 >> 0) & 1;          /* row 0 of column 2 */
    int b = (col1 >> 1) & 1;          /* row 1 of column 1 */
    int c = (col3 >> 2) & 1;          /* row 2 of column 0 */
    return !bad_pattern(a, b, c);
}

/* Initialise the adjacency tables once.  Called lazily from `a`. */
static void init_adjacency(void)
{
    static bool ready = false;
    if (ready) return;
    ready = true;

    for (int i = 0; i < STATES; ++i) {
        int p2 = i / STATE_COLS;          /* second-from-last column */
        int p1 = i % STATE_COLS;          /* last column         */
        toCount[i] = 0;
        for (int p3 = 0; p3 < STATE_COLS; ++p3) {
            if (transition_ok(p2, p1, p3))
                toNext[i][toCount[i]++] = p3;
        }
    }
}

/* Ensure the cache array can hold at least up to `n`. */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        /* start with a minimal allocation */
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* ---------- Public API ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not part of the OEIS offset */

    init_adjacency();
    ensure_cache_size(n);

    /* Initialise DP state the first time we run. */
    if (computed == 0) {
        for (int i = 0; i < STATES; ++i)
            dp_cur[i] = (seq_t)1;   /* every pair of columns is possible */
    }

    /* Compute terms up to n, extending from the current `computed` value. */
    for (idx_t step = computed + 1; step <= n; ++step) {
        /* clear next state */
        memset(dp_next, 0, sizeof(dp_next));

        /* propagate transitions */
        for (int idx = 0; idx < STATES; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            int p1 = idx % STATE_COLS;  /* last column of the state */
            int outc = toCount[idx];
            for (int k = 0; k < outc; ++k) {
                int p3 = toNext[idx][k];
                int next_idx = p1 * STATE_COLS + p3;
                dp_next[next_idx] += val;
            }
        }

        /* accumulate the count for this step (length = 3 + step - 1) */
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i)
            total += dp_next[i];
        cache[(size_t)step] = total;

        /* prepare for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    computed = n;
    return cache[(size_t)n];
}
