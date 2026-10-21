/*
 * A078128  Number of ways to write n as sum of cubes > 1.
 * https://oeis.org/A078128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 903, u16 n <= 2932, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into cubes of integers ≥ 2.
 * Computation: simple dynamic programming (coin‑change style) using all
 * cubes c = m³ with m ≥ 2 and m³ ≤ n.
 * Complexity: O(n · ∛n) per call, well below the time limit for n up to 10⁴.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Memoized DP array.  dp[i] holds the number of ways to write i as sum of cubes. */
    static seq_t *dp = NULL;        /* dynamic array of seq_t */
    static idx_t cap = 0;           /* current allocated size (dp[0..cap-1]) */

    /* Always recompute up to the requested n; n is modest (≤ 10000 in tests). */
    size_t newcap = (size_t)n + 1;
    seq_t *newdp = (seq_t*)malloc(newcap * sizeof(seq_t));
    if (newdp == NULL) return 0;    /* fallback – should not happen in tests */

    memset(newdp, 0, newcap * sizeof(seq_t));
    newdp[0] = 1;   /* one way to make 0 (empty sum) */

    /* Enumerate all cubes m³ (m ≥ 2) not exceeding n. */
    for (idx_t m = 2; ; ++m) {
        idx_t cube = m * m * m;   /* m³ fits in idx_t for n ≤ 10000 */
        if (cube > n) break;
        for (idx_t i = cube; i <= n; ++i) {
            newdp[i] = newdp[i] + newdp[i - cube];
        }
    }

    /* Replace old dp with newdp (freeing previous memory). */
    free(dp);
    dp  = newdp;
    cap = newcap;

    return dp[n];
}
