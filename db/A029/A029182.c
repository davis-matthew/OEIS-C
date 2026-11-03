/*
 * A029182  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 4, 7, and 9
 *            (coefficient of x^n in 1/((1-x^2)(1-x^4)(1-x^7)(1-x^9)))
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *dp = NULL;          /* memoised table of a(k) for k <= dp_cap-1 */
static size_t dp_cap = 0;         /* current size of dp (dp_cap == 0 -> uninitialised) */

static const idx_t PARTS[4] = {2, 4, 7, 9};

/* extend the table so that it contains at least a(n) for the requested n */
static void ensure_up_to(idx_t n)
{
    if (dp_cap > n) return;                      /* already long enough */

    /* allocate a new table of the required size */
    size_t new_cap = (size_t)n + 1;              /* n is small (≤ 10000)       */
    seq_t *new_dp = mem_new(seq_t, new_cap);
    if (!new_dp) exit(1);                        /* out‑of‑memory: abort       */

    /* initialise the DP table using the part sizes */
    new_dp[0] = 1;                               /* empty partition (a(0)=1)   */
    for (int p = 0; p < 4; ++p) {
        idx_t part = PARTS[p];
        for (idx_t i = part; i <= n; ++i) {
            new_dp[i] += new_dp[i - part];
        }
    }

    /* replace the old table */
    mem_free(dp);
    dp      = new_dp;
    dp_cap  = new_cap;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return dp[n];
}
