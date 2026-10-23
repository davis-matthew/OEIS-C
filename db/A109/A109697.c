/*
 * A109697  Number of partitions of n into parts each equal to 1 mod 5.
 * https://oeis.org/A109697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts each equal to 1 mod
 *            5
 * Range:     u8 n <= 66, u16 n <= 181, u32 n <= 563, u64 n <= 1911,
 *            u128 n <= 6898, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* DP table, dp[i] = a(i) */
static idx_t dp_size = 0;         /* allocated size of dp */
static idx_t max_n_computed = 0;  /* highest n for which dp is fully computed */

seq_t a(idx_t n) {
    /* Allocate the DP table if this is the first call */
    if (dp == NULL) {
        dp_size = n > 0 ? n + 1 : 1;
        dp = (seq_t*)malloc(dp_size * sizeof(seq_t));
        if (!dp) return (seq_t)0;         /* out‑of‑memory: return 0 */
        for (idx_t i = 0; i < dp_size; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;
        max_n_computed = 0;
    }

    /* Extend the table to at least n+1 entries if necessary */
    if (n + 1 > dp_size) {
        seq_t *tmp = (seq_t*)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* out‑of‑memory */
        dp = tmp;
        for (idx_t i = dp_size; i <= n; ++i) dp[i] = (seq_t)0;
        dp_size = n + 1;
    }

    /* If we need values beyond what we have already computed,
       recompute the entire DP up to n.  (Good enough for n ≤ 10 000.) */
    if (n > max_n_computed) {
        for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;
        max_n_computed = n;

        /* Classic coin‑change DP: parts are 1,6,11,16,… (all ≡1 mod 5) */
        for (idx_t part = 1; part <= n; part += 5) {
            for (idx_t i = part; i <= n; ++i) {
                dp[i] = dp[i] + dp[i - part];
            }
        }
    }

    return dp[n];
}
