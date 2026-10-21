/*
 * A207372  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 47, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207372 – Number of 7×n 0/1 arrays avoiding
 * 0 0 0 and 0 1 1 horizontally, and 0 0 1 and 0 1 1 vertically.
 *
 * The function a(n) is computed by a transposition‑matrix DP:
 * 1. Enumerate all 7‑bit column patterns that do not contain
 *    the forbidden vertical triples (0 0 1 or 0 1 1).  There are V
 *    such columns (V = 20 for n≥1).
 * 2. For any three consecutive columns i,j,k check that each row
 *    does not create the forbidden horizontal triples (0 0 0 or 0 1 1).
 *    This gives a compatibility table compat[i][j][k] (boolean).
 * 3. Let dp[i][j] be the number of arrays of current width ending with
 *    columns i, j.  For width 2 dp[i][j] = 1 for all valid i,j.
 *    For width > 2, newdp[j][k] += dp[i][j]   if compat[i][j][k] is true.
 *    After computing newdp we swap it with dp.
 * 4. a(n) = sum(dp[i][j]) for all i,j after n columns are processed.
 *
 * This uses O(V²) state (V ≤ 128, in practice 20) and O(n)
 * time.  The algorithm is language‑independent and works for any
 * integer type seq_t provided by the build.  It also respects the
 * unsigned arithmetic rules requested in the specification.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Number of bits per column (7 rows) */
#define BITS     7
/* Maximum possible columns (2^BITS) */
#define MAX_COLS (1 << BITS)

/* A single column value (0..2^7-1) */
static uint8_t cols[MAX_COLS];
/* Compatibility table for three consecutive columns:
 * compat[i][j][k] == 1 iff columns i, j, k satisfy
 * the horizontal forbidden patterns (0 0 0, 0 1 1). */
static char compat[MAX_COLS][MAX_COLS][MAX_COLS];

/* DP arrays: dp and newdp store counts for last two columns.
 * The actual size used is V*V where V is the number of valid columns. */
static seq_t *dp     = NULL;
static seq_t *newdp  = NULL;

/* Cache of computed a(k) values.
 * cache[0] = a(1), cache[1] = a(2), … */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;   /* number of values stored */
static idx_t cache_cap   = 0;   /* allocated capacity */

/* Number of valid columns (<= MAX_COLS) */
static idx_t V = 0;

/* Helper to grow an array of seq_t */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap ? cache_cap * 2 : 4);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Helper to grow dp arrays when V increases (only once) */
static void init_dp_arrays(void)
{
    size_t sz = (size_t)V * (size_t)V;
    dp    = (seq_t *)malloc(sz * sizeof(seq_t));
    newdp = (seq_t *)malloc(sz * sizeof(seq_t));
}

/* Function to fill the columns array and compatibility table. */
static void build_matrices(void)
{
    /* Enumerate all valid columns. */
    V = 0;
    for (int col = 0; col < MAX_COLS; ++col) {
        int ok = 1;
        /* Check all 5 vertical triplets in the 7‑bit column. */
        for (int i = 0; i < BITS - 2; ++i) {
            int trip = (col >> i) & 7;   /* bits i,i+1,i+2 */
            /* forbidden vertical patterns 0 0 1 (4) or 0 1 1 (6) */
            if (trip == 4 || trip == 6) { ok = 0; break; }
        }
        if (ok) { cols[V++] = (uint8_t)col; }
    }

    /* Compute compatibility for every triple of columns. */
    for (int i = 0; i < V; ++i) {
        uint8_t ci = cols[i];
        for (int j = 0; j < V; ++j) {
            uint8_t cj = cols[j];
            for (int k = 0; k < V; ++k) {
                uint8_t ck = cols[k];
                int ok = 1;
                /* For each row check horizontal triples. */
                for (int r = 0; r < BITS; ++r) {
                    int b0 = (ci >> r) & 1;
                    int b1 = (cj >> r) & 1;
                    int b2 = (ck >> r) & 1;
                    int trip = b0 | (b1 << 1) | (b2 << 2); /* 0 0 0 -> 0, 0 1 1 -> 6 */
                    if (trip == 0 || trip == 6) { ok = 0; break; }
                }
                compat[i][j][k] = ok;
            }
        }
    }

    init_dp_arrays();
}

/* Main function that returns a(n). */
seq_t a(idx_t n)
{
    /* One‑time initialisation. */
    if (!cache) {
        build_matrices();
        /* Initialise cache with first two terms. */
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Extend cache until we have computed a(n). */
    while (cache_len < n) {
        idx_t cur = cache_len + 1;  /* 1‑based index of new term */
        if (cur == 1) {
            /* a(1) = V (number of valid columns). */
            seq_t val = (seq_t)V;
            grow_cache();
            cache[cache_len++] = val;
        } else if (cur == 2) {
            /* a(2) = V * V (any two columns). */
            seq_t val = (seq_t)V * (seq_t)V;
            grow_cache();
            cache[cache_len++] = val;

            /* Initialise dp for width 2: each pair i,j counts once. */
            size_t sz = (size_t)V * (size_t)V;
            for (size_t idx = 0; idx < sz; ++idx) dp[idx] = 0;
            for (idx_t i = 0; i < V; ++i)
                for (idx_t j = 0; j < V; ++j)
                    dp[i * V + j] = (seq_t)1;
        } else {
            /* cur >= 3: extend DP to width cur. */
            size_t sz = (size_t)V * (size_t)V;
            /* Clear newdp. */
            for (size_t idx = 0; idx < sz; ++idx) newdp[idx] = 0;

            /* Transition from width cur-1 to cur. */
            for (idx_t i = 0; i < V; ++i) {
                for (idx_t j = 0; j < V; ++j) {
                    size_t idx = (size_t)i * V + j;
                    seq_t val = dp[idx];
                    if (!val) continue;
                    for (idx_t k = 0; k < V; ++k) {
                        if (compat[i][j][k]) {
                            newdp[(size_t)j * V + k] += val;
                        }
                    }
                }
            }

            /* Swap dp and newdp. */
            seq_t *tmp = dp; dp = newdp; newdp = tmp;

            /* Sum all entries to get a(cur). */
            seq_t total = 0;
            for (size_t idx = 0; idx < sz; ++idx) total += dp[idx];

            grow_cache();
            cache[cache_len++] = total;
        }
    }

    return cache[n - 1];
}
