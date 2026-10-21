/*
 * A234660  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays of digits 0..6 in which * every
 *            2×2 subblock has its diagonal sum differing from its *
 *            antidiagonal sum by exactly 4. The value is obtained by *
 *            dynamic programming over the 343 possible 3‑digit rows
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ensure_cache.h"   // use ensure_cache from the utils library

static const idx_t STATES = 343;   /* 7^3 possible rows */

/* adjacency information (s -> t) with s,t in [0,STATES) */
static bool adj_ready = false;
static idx_t *adj_start = NULL;   /* size STATES+1 */
static idx_t *adj_list  = NULL;   /* size num_edges */

/* DP arrays */
static seq_t *dp0 = NULL, *dp1 = NULL;
static idx_t dp_size = STATES;

/* cache for a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_n_cached = 0;

/* build adjacency list once */
static void build_adj(void) {
    if (adj_ready) return;
    int pow7_1 = 7;
    int pow7_2 = 49;

    uint32_t *state_counts = (uint32_t *)malloc(STATES * sizeof(uint32_t));
    size_t total_edges = 0;

    /* count outgoing edges for each state */
    for (idx_t s = 0; s < STATES; ++s) {
        int x_s = s / pow7_2;
        int y_s = (s / pow7_1) % 7;
        int z_s = s % 7;
        uint32_t cnt = 0;
        for (idx_t t = 0; t < STATES; ++t) {
            int x_t = t / pow7_2;
            int y_t = (t / pow7_1) % 7;
            int z_t = t % 7;

            /* column pair 1-2 */
            int diff1 = (x_s + y_t) - (y_s + x_t);
            if (diff1 < 0) diff1 = -diff1;
            if (diff1 != 4) continue;

            /* column pair 2-3 */
            int diff2 = (y_s + z_t) - (z_s + y_t);
            if (diff2 < 0) diff2 = -diff2;
            if (diff2 != 4) continue;

            ++cnt;
        }
        state_counts[s] = cnt;
        total_edges += cnt;
    }

    /* prefix sums for start indices */
    adj_start = (idx_t *)malloc((STATES + 1) * sizeof(idx_t));
    adj_start[0] = 0;
    for (idx_t s = 0; s < STATES; ++s) {
        adj_start[s + 1] = adj_start[s] + state_counts[s];
    }

    /* allocate adjacency list */
    adj_list = (idx_t *)malloc(total_edges * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t s = 0; s < STATES; ++s) {
        int x_s = s / pow7_2;
        int y_s = (s / pow7_1) % 7;
        int z_s = s % 7;
        for (idx_t t = 0; t < STATES; ++t) {
            int x_t = t / pow7_2;
            int y_t = (t / pow7_1) % 7;
            int z_t = t % 7;

            int diff1 = (x_s + y_t) - (y_s + x_t);
            if (diff1 < 0) diff1 = -diff1;
            if (diff1 != 4) continue;

            int diff2 = (y_s + z_t) - (z_s + y_t);
            if (diff2 < 0) diff2 = -diff2;
            if (diff2 != 4) continue;

            adj_list[pos++] = t;
        }
    }

    free(state_counts);
    adj_ready = true;
}

/* initialise DP buffers once */
static void init_dp(void) {
    if (dp0 != NULL) return;
    dp0 = (seq_t *)malloc(dp_size * sizeof(seq_t));
    dp1 = (seq_t *)malloc(dp_size * sizeof(seq_t));
    /* start with all states count 1 (zero transitions) */
    for (idx_t i = 0; i < dp_size; ++i) dp0[i] = 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* outside offset, return dummy */

    /* ensure adjacency and DP buffers */
    build_adj();
    init_dp();

    /* extend cache if needed */
    if (n > max_n_cached) {
        /* compute up to n and store values */
        for (idx_t step = max_n_cached + 1; step <= n; ++step) {
            /* zero next dp array */
            memset(dp1, 0, dp_size * sizeof(seq_t));

            /* propagate transitions */
            for (idx_t s = 0; s < dp_size; ++s) {
                seq_t cnt = dp0[s];
                if (cnt == 0) continue;
                idx_t start = adj_start[s];
                idx_t end   = adj_start[s + 1];
                for (idx_t e = start; e < end; ++e) {
                    idx_t t = adj_list[e];
                    dp1[t] += cnt;
                }
            }

            /* sum all counts to get a(step) */
            seq_t total = 0;
            for (idx_t t = 0; t < dp_size; ++t) total += dp1[t];

            /* store in cache */
            if (step >= (idx_t)cache_cap) {
                ensure_cache(step, &cache, &cache_cap);
            }
            cache[step] = total;

            /* swap dp arrays for next iteration */
            seq_t *tmp = dp0;
            dp0 = dp1;
            dp1 = tmp;
        }
        max_n_cached = n;
    }

    return cache[n];
}
