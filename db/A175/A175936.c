/*
 * A175936  Number of lattice walks from (0,0) to (n,n) using steps
 *          S={(k,0),(0,k),(r,r)|0<k<=3,0<r<=2} which never go above the line
 *          y=x.
 * https://oeis.org/A175936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175936: Number of lattice walks from (0,0) to (n,n) using steps
 * {(k,0),(0,k),(r,r)} with 0<k≤3, 0<r≤2 that never go above the line y=x.
 *
 * The recurrence is based on coordinates (x,y) with y≤x.
 * Let dp[x][y] = number of walks from (0,0) to (x,y) staying below y=x.
 * Recurrence:
 *   dp[0][0] = 1
 *   for x≥0, 0≤y≤x:
 *     dp[x][y] = Σ_{k=1..3, x≥k}   dp[x-k][y]          // horizontal
 *             + Σ_{k=1..3, y≥k}   dp[x][y-k]          // vertical
 *             + Σ_{r=1..2, y≥r}   dp[x-r][y-r]        // diagonal
 * The desired term is a(n) = dp[n][n].
 *
 * The implementation uses three previous rows and one current row as
 * circular buffers.  Values are stored in a static cache so that the
 * function works for any call order and is O(n²) time with O(n) memory.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache[n] = dp[n][n]   */
static size_t cache_cap = 0;         /* allocated cache size */
static idx_t computed_x = 0;         /* largest x already computed */

static seq_t *prev[3] = {NULL, NULL, NULL}; /* prev[0]=row x-1, prev[1]=row x-2, prev[2]=row x-3 */
static seq_t *cur = NULL;            /* buffer for current row */

/* Grow all arrays to hold indices up to n (size n+1). */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (need <= cache_cap) return;

    size_t old = cache_cap;
    cache_cap = need;

    /* Reallocate cache */
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    if (old < cache_cap)
        memset(&cache[old], 0, (cache_cap - old) * sizeof(seq_t));

    /* Reallocate row buffers */
    for (int r = 0; r < 3; ++r) {
        tmp = (seq_t *)realloc(prev[r], cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        prev[r] = tmp;
        if (old < cache_cap)
            memset(&prev[r][old], 0, (cache_cap - old) * sizeof(seq_t));
    }

    /* Reallocate current buffer */
    tmp = (seq_t *)realloc(cur, cache_cap * sizeof(seq_t));
    if (!tmp) abort();
    cur = tmp;
    if (old < cache_cap)
        memset(&cur[old], 0, (cache_cap - old) * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (cache_cap == 0) {           /* first call: initialise everything */
        ensure_capacity(0);
        cur[0] = (seq_t)1;
        cache[0] = (seq_t)1;

        /* Rotate buffers so that prev[0] points to row 0 */
        seq_t *tmp = cur;           /* old cur (row 0) */
        cur = prev[2];              /* cur becomes old prev[2] */
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = tmp;              /* prev[0] = row 0 */

        computed_x = 0;
    }

    ensure_capacity(n);

    /* Compute rows for x = computed_x + 1 .. n */
    for (idx_t x = computed_x + 1; x <= n; ++x) {
        /* For each y from 0 to x compute dp[x][y] */
        for (idx_t y = 0; y <= x; ++y) {
            seq_t val = 0;

            /* Horizontal steps from previous rows */
            if (x >= 1) val += prev[0][y];
            if (x >= 2) val += prev[1][y];
            if (x >= 3) val += prev[2][y];

            /* Vertical steps within current row */
            if (y >= 1) val += cur[y - 1];
            if (y >= 2) val += cur[y - 2];
            if (y >= 3) val += cur[y - 3];

            /* Diagonal steps from previous rows */
            if (y >= 1) val += prev[0][y - 1];
            if (y >= 2) val += prev[1][y - 2];

            cur[y] = val;
        }

        cache[x] = cur[x];   /* store a(x) = dp[x][x] */

        /* Rotate buffers for next iteration */
        seq_t *tmp = cur;
        cur = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = tmp;
    }

    computed_x = n;
    return cache[n];
}
