/*
 * A025770  Expansion of 1/((1-x)*(1-x^3)*(1-x^10)).
 * https://oeis.org/A025770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025770: number of partitions of n into parts 1, 3 and 10.
   Formula (used to verify correctness):
      a(n) = floor((n² + 14n + 75 - 6·(n mod 3))/60).  */

/* Use a simple coin‑change DP; the array is recomputed on each call
   but the cost is only 3·n operations, which is trivial for n≤10000.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Allocate (or grow) a temporary table of size n+1.  */
    size_t needed = (size_t)n + 1;
    static seq_t *dp    = NULL;   /* cache for the DP table */
    static size_t cap   = 0;      /* current capacity of dp */

    if (needed > cap) {
        seq_t *newdp = alloc_seq_array(needed);
        if (!newdp)           /* allocation failed – return 0 */
            return 0;
        if (dp)                /* free old table if present */
            seq_free_array(dp);
        dp  = newdp;
        cap = needed;
    }

    /* Initialise the table.  */
    dp[0] = 1;                   /* one empty partition of 0 */
    for (size_t i = 1; i < needed; ++i)
        dp[i] = 0;

    /* The available parts.  */
    const idx_t parts[3] = {1, 3, 10};
    const size_t num_parts = 3;

    /* Standard unbounded‑coin change DP.  */
    for (size_t p = 0; p < num_parts; ++p) {
        idx_t coin = parts[p];
        for (idx_t sum = coin; sum < (idx_t)needed; ++sum) {
            dp[sum] += dp[sum - coin];
        }
    }

    return dp[n];
}
