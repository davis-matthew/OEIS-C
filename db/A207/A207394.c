/*
 * A207394  Number of 5Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 5×n binary arrays avoiding * horizontally: 001 and 010
 *            * vertically: 001 and 110 * Computed by dynamic programming over
 *            admissible column states
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   horiz_valid, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/horiz_valid.h"

/* ------------------------------------------------------------------ */

#define MAX_COLS   32   /* maximum possible columns (2^5) */
#define MAX_STATES (MAX_COLS * MAX_COLS)

/* Admissible columns and transition tables */
static int  legal_cols[MAX_COLS];
static int  trans_next[MAX_STATES][MAX_COLS];
static unsigned char trans_cnt[MAX_STATES];
static int   L = 0;                /* number of legal columns          */
static size_t state_sz = 0;        /* L * L                            */

/* DP arrays (current and next step) */
static seq_t *dp   = NULL;
static seq_t *dp_n = NULL;

/* Cached sequence values: cached[i] = a(i) for 0 <= i <= cache_len */
static seq_t *cached = NULL;
static size_t cache_len = 0;   /* highest index currently cached      */
static idx_t  max_computed = 0;/* highest n that has been computed     */

/* ------------------------------------------------------------------ */

/* A column pattern 'pat' (5 bits) is valid if it does not contain
 * the vertical patterns 001 (1) or 110 (6) in any consecutive triple.
 */
static bool col_valid(int pat)
{
    for (int st = 0; st < 3; ++st) {
        int tri = (pat >> st) & 0x7;
        if (tri == 0x1 || tri == 0x6)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */

/* One-time initialization: compute legal columns, transition tables,
 * initialise DP arrays and cache for a(1) and a(2).
 */
static void init(void)
{
    /* 1. List legal columns */
    L = 0;
    for (int pat = 0; pat < 32; ++pat)
        if (col_valid(pat))
            legal_cols[L++] = pat;

    /* 2. Build adjacency tables */
    state_sz = (size_t)L * (size_t)L;
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            int state = i * L + j;
            trans_cnt[state] = 0;
            for (int k = 0; k < L; ++k) {
                if (horiz_valid(legal_cols[i], legal_cols[j], legal_cols[k])) {
                    trans_next[state][trans_cnt[state]++] = k;
                }
            }
        }
    }

    /* 3. Allocate DP arrays */
    dp   = seq_alloc_new_array(state_sz);
    dp_n = seq_alloc_new_array(state_sz);

    /* For step 2 (two columns) each state has one arrangement */
    for (size_t i = 0; i < state_sz; ++i)
        dp[i] = 1;

    /* 4. Initialise cache */
    cached = seq_alloc_new_array(3);   /* indices 0..2 */
    cached[0] = 0;
    cached[1] = (seq_t)L;
    cached[2] = (seq_t)L * (seq_t)L;

    max_computed = 2;
}

/* ------------------------------------------------------------------ */

/* Expand the cache array to include terms up to 'new_len' (inclusive) */
static void expand_cache(size_t new_len)
{
    if (new_len <= cache_len)
        return;

    seq_t *tmp = seq_alloc_new_array(new_len + 1);
    for (size_t i = 0; i <= cache_len; ++i)
        tmp[i] = cached[i];
    free(cached);
    cached = tmp;
    cache_len = new_len;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */

    if (!dp)                  /* lazy initialization */
        init();

    if (n <= max_computed)
        return cached[n];

    /* Extend computation up to n */
    expand_cache((size_t)n);

    for (idx_t step = max_computed + 1; step <= n; ++step) {
        /* Zero next DP array */
        for (size_t i = 0; i < state_sz; ++i)
            dp_n[i] = 0;

        /* Propagate transitions among states */
        for (int state = 0; state < (int)state_sz; ++state) {
            seq_t cnt = dp[state];
            if (cnt == 0)
                continue;

            int cur = state % L;
            unsigned char tcnt = trans_cnt[state];

            for (unsigned char t = 0; t < tcnt; ++t) {
                int nxt = trans_next[state][t];
                int new_state = cur * L + nxt;
                dp_n[new_state] += cnt;
            }
        }

        /* Sum all states to get total a(step) */
        seq_t total = 0;
        for (size_t i = 0; i < state_sz; ++i)
            total += dp_n[i];
        cached[step] = total;

        /* Prepare for next step */
        seq_t *tmp = dp;
        dp = dp_n;
        dp_n = tmp;

        max_computed = step;
    }

    return cached[n];
}
