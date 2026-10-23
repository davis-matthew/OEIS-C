/*
 * A036034  Number of partitions of n into parts not of form 4k+2, 24k, 24k+11
 *          or 24k-11. Also number of partitions in which no odd part is
 *          repeated, with at most 5 parts of size less than or equal to 2 and
 *          where differences between parts at distance 5 are greater than 1
 *          when the smallest part is odd and gre...
 * https://oeis.org/A036034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of form 4k+2,
 *            24k, 24k+11, or 24k-11, using a one‑dimensional DP
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   alloc_seq_array, forbidden_part, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/forbidden_part.h"

static seq_t *dp = NULL;      /* DP table for cumulative results */
static idx_t dp_size = 0;     /* allocated capacity for dp */

/* Main routine for the OEIS sequence A036034. */
seq_t a(idx_t n)
{
    if (n == 0)                    /* offset starts at 1; 0 is unused */
        return 0;

    /* Allocate or grow the DP table to handle up to n. */
    if (dp_size <= n) {
        if (dp)
            free_seq_array(dp);
        dp = alloc_seq_array((size_t)(n + 1));
        dp_size = n + 1;
    }

    /* Reset entries that will be recomputed. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;                      /* one way to make 0 */

    /* Populate DP table. */
    for (idx_t p = 1; p <= n; ++p) {
        if (forbidden_part(p))
            continue;
        for (idx_t s = p; s <= n; ++s)
            dp[s] += dp[s - p];
    }
    return dp[n];
}
