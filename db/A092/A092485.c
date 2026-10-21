/*
 * A092485  Expansion of Product_{m>=1} (1+m*(m+1)*q^m).
 * https://oeis.org/A092485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{m≥1} (1 + m*(m+1) q^m)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 29, u64 n <= 75, u128 n <= 198,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (OEIS A092485)                                                   */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_seq_array.h"

/* Persistent DP array re-used between calls for efficiency. */
static seq_t *dp = NULL;
static size_t dp_cap = 0;

/*  Computes a(n) for any n ≥ 0.  */
seq_t a(idx_t n)
{
    /* Ensure dp has space for indices 0 … n. */
    size_t need = (size_t)n + 1;
    if (dp_cap < need)
        grow_seq_array(&dp, &dp_cap, need);

    /* Initialise coefficients for this n. */
    dp[0] = 1;
    for (size_t i = 1; i <= (size_t)n; ++i)
        dp[i] = 0;

    /* 0/1 knapsack DP over distinct part sizes.  */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t w = (seq_t)m * ((seq_t)m + 1);      /* weight for part m   */
        for (idx_t s = n; s >= m; --s) {
            dp[(size_t)s] += dp[(size_t)(s - m)] * w;
        }
    }

    return dp[(size_t)n];
}
