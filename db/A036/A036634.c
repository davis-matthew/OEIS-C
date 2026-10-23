/*
 * A036634  Number of 5-ary rooted trees with n nodes and height exactly 3.
 * https://oeis.org/A036634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A036634: Number of 5‑ary rooted trees with n nodes and height exactly 3 */
/* A(n) = #{ rooted trees with at most 5 children per node, height = 3 (nodes), total nodes = n } */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 31   /* maximum n for which a(n) can be non‑zero (1 + 5·6 = 31) */

static seq_t a_cache[MAX_N + 1];
static bool initialized = false;

static void init_sequence(void)
{
    if (initialized) return;

    /* Count of trees of height ≤ 2 for each size (1 ≤ n ≤ 6) */
    seq_t G2[MAX_N + 1] = {0};
    for (idx_t i = 1; i <= 6; ++i) G2[i] = 1;

    /* DP over child multisets.
       dp[used][sum] – number of distinct unordered multisets of child sub‑trees
       (each of which is a tree of height ≤ 2) that use 'used' children and
       have a total of 'sum' nodes. 0 ≤ used ≤ 5, 0 ≤ sum ≤ MAX_N‑1. */
    seq_t dp[6][MAX_N] = {0};
    seq_t newdp[6][MAX_N];
    dp[0][0] = 1;

    /* Types of child sub‑trees have sizes 1..6 (their own trees of height ≤ 2). */
    const idx_t types[6] = {1, 2, 3, 4, 5, 6};

    for (int t = 0; t < 6; ++t) {
        idx_t w = types[t];
        /* Start with existing combinations */
        for (int u = 0; u <= 5; ++u)
            for (idx_t s = 0; s < MAX_N; ++s)
                newdp[u][s] = dp[u][s];

        /* Add k copies of the current type (k ≥ 1, up to 5‑1‑used). */
        for (int used = 0; used <= 5; ++used) {
            for (idx_t sum = 0; sum < MAX_N; ++sum) {
                seq_t cur = dp[used][sum];
                if (!cur) continue;
                int maxk = 5 - used;
                for (int k = 1; k <= maxk; ++k) {
                    idx_t new_sum = sum + (idx_t)(k * w);
                    if (new_sum >= MAX_N) break;
                    newdp[used + k][new_sum] += cur;
                }
            }
        }
        /* Copy newdp back to dp for next type */
        for (int u = 0; u <= 5; ++u)
            for (idx_t s = 0; s < MAX_N; ++s)
                dp[u][s] = newdp[u][s];
    }

    /* Compute a(n) = F3(n) – G2(n) */
    for (idx_t n = 0; n <= MAX_N; ++n) {
        seq_t F3 = 0;
        if (n == 0) {
            a_cache[n] = 0;
            continue;
        }
        if (n == 1) {
            F3 = 1;               /* single root */
        } else {
            idx_t need = n - 1;   /* nodes contributed by the children */
            for (int used = 1; used <= 5; ++used)
                F3 += dp[used][need];
        }
        a_cache[n] = F3 - G2[n];
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n > MAX_N) return 0;
    return a_cache[n];
}
