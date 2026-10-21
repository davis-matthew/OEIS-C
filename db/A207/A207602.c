/*
 * A207602  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n 0/1 arrays avoiding the patterns
 *            horizontally 0 0 0 and 0 1 0, and vertically 0 0 1 and 1 0 1. No
 *            closed‑form formula; computed with a transfer‑matrix DP. Initial
 *            values: a(1)=10, a(2)=100
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Compile–time constants; use enum so they are true constants
       in C.  The values are small enough to keep memory usage modest.
       ------------------------------------------------------------------ */
    enum { ROWS = 5, COLS = 32, STATES = COLS * COLS };

    /* ------------------------------------------------------------------
       Static data structures holding the transition tables.
       They are computed once on first entry.
       ------------------------------------------------------------------ */
    struct init_data {
        bool                initialized;
        int                 valid[COLS];                 /* column is vertically valid? */
        int                 valid_list[COLS];             /* list of valid columns */
        int                 valid_count;                  /* size of valid_list        */
        int                 next_count[STATES];           /* number of transitions   */
        int                 next_list[STATES][COLS];      /* transition state indices */
    } static init = {0};

    /* ------------------------------------------------------------------
       Simple cache storing already‑computed a(k) values.
       ------------------------------------------------------------------ */
    struct cache_data {
        idx_t   cur_len;   /* highest n computed (1‑based)   */
        size_t  cap;       /* allocated size of cache array   */
        seq_t*  cache;     /* array of a(1)…a(cur_len)      */
    } static cache = {0, 0, NULL};

    /* ------------------------------------------------------------------
       DP arrays for the last two columns' states.
       ------------------------------------------------------------------ */
    static seq_t dp[STATES];
    static seq_t dp_next[STATES];

    /* ------------------------------------------------------------------
       Build the transition tables on first call.
       ------------------------------------------------------------------ */
    if (!init.initialized) {
        /* Precompute which columns are vertically valid. */
        int vcnt = 0;
        for (int col = 0; col < COLS; ++col) {
            bool ok = true;
            for (int r = 0; r <= ROWS - 3; ++r) {
                int a = (col >> r)     & 1;
                int b = (col >> (r+1)) & 1;
                int c = (col >> (r+2)) & 1;
                if ((a==0 && b==0 && c==1) || (a==1 && b==0 && c==1)) {
                    ok = false;
                    break;
                }
            }
            init.valid[col] = ok;
            if (ok) init.valid_list[vcnt++] = col;
        }
        init.valid_count = vcnt;

        /* Precompute horizontal transitions. */
        for (int i = 0; i < vcnt; ++i) {
            int c1 = init.valid_list[i];
            for (int j = 0; j < vcnt; ++j) {
                int c2 = init.valid_list[j];
                int src = c1 * COLS + c2;
                int tcnt = 0;
                for (int k = 0; k < vcnt; ++k) {
                    int c3 = init.valid_list[k];
                    bool ok = true;
                    for (int r = 0; r < ROWS; ++r) {
                        int a = (c1 >> r) & 1;
                        int b = (c2 >> r) & 1;
                        int c = (c3 >> r) & 1;
                        if ((a==0 && b==0 && c==0) || (a==0 && b==1 && c==0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) init.next_list[src][tcnt++] = (c2 * COLS + c3);
                }
                init.next_count[src] = tcnt;
            }
        }
        init.initialized = true;
    }

    /* ------------------------------------------------------------------
       Resize the cache if we need to store a larger n.
       ------------------------------------------------------------------ */
    if (cache.cap < (size_t)n) {
        size_t new_cap = cache.cap ? cache.cap << 1 : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache.cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failure */
        cache.cache = tmp;
        cache.cap = new_cap;
    }

    /* ------------------------------------------------------------------
       Return a cached value if already known.
       ------------------------------------------------------------------ */
    if (n <= cache.cur_len) {
        return cache.cache[n-1];
    }

    /* ------------------------------------------------------------------
       Compute a(1) if not yet done.
       ------------------------------------------------------------------ */
    if (cache.cur_len == 0) {
        cache.cache[0] = (seq_t)init.valid_count;   /* a(1) */
        cache.cur_len = 1;
        if (n == 1) return cache.cache[0];
    }

    /* ------------------------------------------------------------------
       Compute a(2) if not yet done.
       ------------------------------------------------------------------ */
    if (cache.cur_len == 1 && n >= 2) {
        cache.cache[1] = (seq_t)init.valid_count * init.valid_count;
        cache.cur_len = 2;
        if (n == 2) return cache.cache[1];
    }

    /* ------------------------------------------------------------------
       Initialise DP for length 2 (all state pairs count as 1).
       ------------------------------------------------------------------ */
    if (cache.cur_len == 2) {
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i < init.valid_count; ++i) {
            int c1 = init.valid_list[i];
            for (int j = 0; j < init.valid_count; ++j) {
                int c2 = init.valid_list[j];
                int s  = c1 * COLS + c2;
                dp[s] = 1;
            }
        }
    }

    /* ------------------------------------------------------------------
       Extend DP for lengths cur_len+1 … n.
       ------------------------------------------------------------------ */
    for (idx_t L = cache.cur_len + 1; L <= n; ++L) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            int trans_cnt = init.next_count[s];
            int *trans = init.next_list[s];
            for (int k = 0; k < trans_cnt; ++k) {
                int t = trans[k];
                dp_next[t] += val;
            }
        }

        /* Sum over all states to get a(L). */
        seq_t total = 0;
        for (int s = 0; s < STATES; ++s) total += dp_next[s];
        cache.cache[L-1] = total;

        /* Prepare for next iteration. */
        memcpy(dp, dp_next, sizeof(dp));
        cache.cur_len = L;
    }

    return cache.cache[n-1];
}
