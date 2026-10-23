/*
 * A187597  Number of (n+2)X3 0..2 arrays with no 2X2 subblock sum equal to
 *          any diagonal or antidiagonal neighbor 2X2 subblock sum.
 * https://oeis.org/A187597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   precompute_triples
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Count the number of (n+2)×3 arrays with entries 0,1,2 such that
 * no 2×2 subblock has a sum equal to the sum of any of its diagonal
 * neighbours (i.e. the 2×2 blocks that overlap in a single cell).
 *
 * The computation is performed by dynamic programming over the rows.
 * For each pair of consecutive rows we keep the number of ways to build
 * the array up to that point.  The transition to a new row uses the
 * pre‑computed list of admissible next rows given the previous two rows.
 * Results are memoised in an array so that repeated calls with different
 * values of n are efficient.
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/precompute_triples.h"

static inline int state_index(int prev, int last) { return prev * 27 + last; }

/* Memoised results for a(k) for k >= 1. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can hold at least (n+1) entries. */
static void ensure_cache_size(idx_t n) {
    if (n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure: terminate. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
    /* Initialise new slots to 0 which indicates 'not computed'. */
    if (cache_len < new_cap) {
        memset(cache + cache_len, 0, (new_cap - cache_len) * sizeof(seq_t));
        cache_len = new_cap;
    }
}

/* Core function returning a(n). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* Not used (offset 1). */

    /* Static DP state */
    static bool initialized      = false;
    static int trans[729][27];
    static unsigned char transcnt[729];
    static seq_t dp[729];
    static seq_t newdp[729];
    static idx_t processed_n = 0;           /* number of rows added beyond 2 */

    if (!initialized) {
        precompute_triples(trans, transcnt);
        memset(dp, 0, sizeof(dp));
        for (int a = 0; a < 27; ++a)
            for (int b = 0; b < 27; ++b)
                dp[state_index(a, b)] = (seq_t)1;  /* two rows */
        cache = NULL;
        cache_cap = cache_len = 0;
        processed_n = 0;
        initialized = true;
    }

    ensure_cache_size(n);
    if (cache[n] != (seq_t)0) return cache[n];

    while (processed_n < n) {
        /* Add one more row to the array. */
        memset(newdp, 0, sizeof(newdp));
        for (int prev = 0; prev < 27; ++prev) {
            for (int last = 0; last < 27; ++last) {
                int idx = state_index(prev, last);
                seq_t val = dp[idx];
                if (val == (seq_t)0) continue;
                unsigned char cnt = transcnt[idx];
                for (int k = 0; k < cnt; ++k) {
                    int next_row = trans[idx][k];
                    int new_idx = state_index(last, next_row);
                    newdp[new_idx] += val;
                }
            }
        }
        memcpy(dp, newdp, sizeof(dp));

        /* Sum all end states to get a(processed_n+1). */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 729; ++i) sum += dp[i];
        processed_n++;
        cache[processed_n] = sum;
    }

    return cache[n];
}
