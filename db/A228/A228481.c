/*
 * A228481  Number of nX7 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically or antidiagonally.
 * https://oeis.org/A228481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the number of n×7 binary arrays with top-left
   value 1 and no two 1's adjacent horizontally, vertically or on the
   antidiagonal.  The computation is done by dynamic programming over
   rows.  All required sub‑ranges are cached so that any order of calls
   is supported. */

#include <stdlib.h>
#include <stdbool.h>

/* Constants for a width of 7 columns.  There are 34 bitmasks of length
   7 with no consecutive 1's. */
#define WIDTH   7
#define MAX_MASKS 34

static int masks[MAX_MASKS];              /* bitmask values            */
static unsigned char trans[MAX_MASKS][MAX_MASKS]; /* transition matrix   */
static int mask_count = 0;                /* actual number of masks    */

/* Precompute masks that contain no consecutive 1s and the transition
   table between successive rows.  This is done only once. */
static void build_tables(void)
{
    /* generate all valid masks */
    for (int m = 0; m < (1 << WIDTH); ++m)
    {
        if ((m & (m << 1)) == 0)
            masks[mask_count++] = m;
    }

    /* compute allowed transitions */
    for (int i = 0; i < mask_count; ++i)
        for (int j = 0; j < mask_count; ++j)
        {
            int prev = masks[i];
            int curr = masks[j];
            /* vertical adjacency forbidden */
            if (prev & curr) {
                trans[i][j] = 0;
                continue;
            }
            /* antidiagonal (prev j+1 -> curr j) forbidden */
            if (curr & (prev >> 1)) {
                trans[i][j] = 0;
                continue;
            }
            trans[i][j] = 1;
        }
}

/* Dynamic programming state that persists across calls.  It keeps
   the current row index, the per‑row dp array, and a cache of the
   sequence values computed so far. */
static idx_t  current_row = 0;                       /* number of rows processed */
static seq_t  dp[MAX_MASKS];                         /* dp for this row      */
static seq_t  dp_next[MAX_MASKS];                    /* buffer for next row  */
static seq_t *cache = NULL;                          /* cache of a(n) values */
static idx_t  cache_cap = 0;                         /* allocated cache size*/
static idx_t  cache_len = 0;                         /* highest n stored    */

seq_t a(idx_t n)
{
    /* Ensure tables are built once */
    static bool tables_ready = false;
    if (!tables_ready) {
        build_tables();
        tables_ready = true;
    }

    /* Expand cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap ? cache_cap * 2 : 64);
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure – unlikely for test data */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[n];

    /* Initialize for the first time we compute a value */
    if (current_row == 0) {
        for (int i = 0; i < mask_count; ++i)
            dp[i] = (masks[i] & 1) ? (seq_t)1 : (seq_t)0; /* first row must start with 1 */

        /* sum of dp gives a(1) */
        seq_t sum = 0;
        for (int i = 0; i < mask_count; ++i) sum += dp[i];
        cache[1] = sum;
        cache_len = 1;
        current_row = 1;
    }

    /* Grow the DP to the required row n */
    while (current_row < n) {
        for (int j = 0; j < mask_count; ++j) dp_next[j] = 0;
        for (int i = 0; i < mask_count; ++i) {
            seq_t val = dp[i];
            if (!val) continue;
            for (int j = 0; j < mask_count; ++j) {
                if (trans[i][j])
                    dp_next[j] += val;
            }
        }
        /* copy next row state to current dp */
        for (int j = 0; j < mask_count; ++j) dp[j] = dp_next[j];
        ++current_row;

        /* compute a(current_row) */
        seq_t sum = 0;
        for (int j = 0; j < mask_count; ++j) sum += dp[j];
        if (current_row > cache_len) {
            cache[current_row] = sum;
            cache_len = current_row;
        }
    }
    return cache[n];
}
