/*
 * A207028  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n 0‑1 arrays avoiding the horizontal patterns
 *            000 and 011 and the vertical patterns 001 and 101. Computation
 *            is by a transfer‑matrix DP on pairs of columns
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 57, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* helpers defined outside the main function ---------------------------------------------------- */
static inline bool column_ok(int col) {          /* no vertical 001 or 101 in this 5‑bit column */
    for (int b = 0; b <= 2; ++b) {
        int a = (col >> b)     & 1;
        int c = (col >> (b+2)) & 1;
        int m = (col >> (b+1)) & 1;
        if (a == 0 && m == 0 && c == 1) return false;  /* 001 */
        if (a == 1 && m == 0 && c == 1) return false;  /* 101 */
    }
    return true;
}

static inline bool vertical_ok(int a, int b, int c) { /* check horizontal triple at each row */
    for (int r = 0; r < 5; ++r) {
        int x = (a >> r) & 1;
        int y = (b >> r) & 1;
        int z = (c >> r) & 1;
        if (x == 0 && y == 0 && z == 0) return false;  /* 000 */
        if (x == 0 && y == 1 && z == 1) return false;  /* 011 */
    }
    return true;
}

/* the actual sequence function -------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* static state that persists between calls ---------------------------------------- */
    static bool init = false;
    static idx_t max_cache = 0;              /* largest n already computed   */
    static idx_t cache_capacity = 0;         /* allocated size of cache array */
    static seq_t *cache = NULL;              /* cache[1..max_cache] values    */

    /* DP data ------------------------------------------------------------ */
    static int valid_cols[32];
    static int num_cols = 0;
    static int state_idx[32][32];
    static int num_states = 0;
    static int *next_state[1024];
    static size_t next_cnt[1024];
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static bool dp_ready = false;

    /* one–time initialization ---------------------------------------------- */
    if (!init) {
        init = true;

        /* collect all columns that satisfy vertical constraints */
        for (int col = 0; col < 32; ++col) {
            if (column_ok(col)) {
                valid_cols[num_cols++] = col;
            }
        }

        /* build the state mapping (column pair -> state id) */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                state_idx[a][b] = -1;
        num_states = 0;
        for (int i = 0; i < num_cols; ++i)
            for (int j = 0; j < num_cols; ++j)
                state_idx[valid_cols[i]][valid_cols[j]] = num_states++;

        /* compute transitions for each state */
        for (int s = 0; s < num_states; ++s) {
            int a_col = valid_cols[s / num_cols];
            int b_col = valid_cols[s % num_cols];
            size_t cnt = 0;
            int *tmp = NULL;

            for (int k = 0; k < num_cols; ++k) {
                int c_col = valid_cols[k];
                if (vertical_ok(a_col, b_col, c_col))
                    ++cnt;
            }
            if (cnt > 0) {
                tmp = (int *)malloc(cnt * sizeof(int));
                size_t p = 0;
                for (int k = 0; k < num_cols; ++k) {
                    int c_col = valid_cols[k];
                    if (vertical_ok(a_col, b_col, c_col))
                        tmp[p++] = state_idx[b_col][c_col];
                }
            }
            next_state[s] = tmp;
            next_cnt[s] = cnt;
        }

        /* initialise DP tables (length 2) */
        dp_cur = (seq_t *)malloc(num_states * sizeof(seq_t));
        dp_next = (seq_t *)malloc(num_states * sizeof(seq_t));
        for (int i = 0; i < num_states; ++i) dp_cur[i] = 1;   /* one way for each pair */
        dp_ready = true;

        /* initialise cache for n=1 and n=2 */
        seq_t *c0 = (seq_t *)malloc(3 * sizeof(seq_t)); /* indices 0,1,2 */
        cache = c0;
        cache_capacity = 2;
        cache[1] = (seq_t)num_cols;
        seq_t sq = (seq_t)num_cols;
        cache[2] = sq * sq;
        max_cache = 2;
    }

    /* extend cache up to requested n -------------------------------- */
    if (n > max_cache) {
        /* enlarge cache if necessary */
        if (cache_capacity < n) {
            seq_t *newc = (seq_t *)malloc((n + 1) * sizeof(seq_t));
            for (idx_t i = 1; i <= max_cache; ++i) newc[i] = cache[i];
            free(cache);
            cache = newc;
            cache_capacity = n;
        }

        /* extend DP from max_cache+1 to n */
        for (idx_t len = max_cache + 1; len <= n; ++len) {
            /* zero dp_next */
            for (int i = 0; i < num_states; ++i) dp_next[i] = 0;

            /* transition */
            for (int s = 0; s < num_states; ++s) {
                seq_t val = dp_cur[s];
                if (val == 0) continue;
                size_t c = next_cnt[s];
                int *t = next_state[s];
                for (size_t k = 0; k < c; ++k) {
                    int ns = t[k];
                    dp_next[ns] = dp_next[ns] + val;
                }
            }

            /* sum all states for the new length */
            seq_t total = 0;
            for (int i = 0; i < num_states; ++i) total = total + dp_next[i];
            cache[len] = total;

            /* swap pointers for next iteration */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }
        max_cache = n;
    }

    return cache[n];
}
