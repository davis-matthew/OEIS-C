/*
 * A199120  Number of partitions of n into terms of (1,4)-Ulam sequence, cf.
 *          A003666.
 * https://oeis.org/A199120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate the (1,4)-Ulam sequence up to n and use the
   classic coin‑change dynamic programming to count partitions of n
   with these parts.  Results are cached so repeated calls are fast. */

#include <stdlib.h>
#include <stdint.h>

/* Cached partition table.  dp_cache[i] = a(i).  cache_size = size of dp_cache. */
static seq_t *dp_cache = NULL;
static idx_t cache_size = 0;

/* Recompute the cache up to (and including) new_max. */
static void recompute_cache(idx_t new_max)
{
    /* Allocate DP array. */
    seq_t *dp = (seq_t *)malloc((size_t)(new_max + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= new_max; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Count array for sums of two earlier distinct Ulam terms. */
    uint32_t *cnt = (uint32_t *)malloc((size_t)(new_max + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= new_max; ++i) cnt[i] = 0;

    /* Array to hold the Ulam sequence terms up to new_max. */
    idx_t *ulam = (idx_t *)malloc((size_t)(new_max + 1) * sizeof(idx_t));
    size_t ulam_len = 0;

    /* Insert the initial seeds 1 and 4 if they are <= new_max. */
    if (new_max >= 1) {
        ulam[ulam_len++] = 1;
    }
    if (new_max >= 4) {
        ulam[ulam_len++] = 4;
        /* Update sum counts with the pair (1,4). */
        if (ulam_len >= 2) {
            idx_t s = ulam[0] + ulam[1];
            if (s <= new_max) ++cnt[s];
        }
    }

    /* Generate the rest of the Ulam sequence. */
    if (ulam_len > 0) {
        idx_t last = ulam[ulam_len - 1];
        idx_t candidate = last + 1;

        while (candidate <= new_max) {
            /* If candidate has exactly one representation as sum of two
               earlier distinct terms, add it to the Ulam set. */
            if (cnt[candidate] == 1) {
                ulam[ulam_len++] = candidate;
                idx_t new_term = candidate;

                /* Update sum counts with all previous terms. */
                for (size_t j = 0; j < ulam_len - 1; ++j) {
                    idx_t s = ulam[j] + new_term;
                    if (s <= new_max) ++cnt[s];
                }

                last = new_term;
                candidate = last + 1;
            } else {
                /* Skip until the next candidate. */
                ++candidate;
            }
        }
    }

    /* Partition DP using the computed Ulam set. */
    for (size_t ui = 0; ui < ulam_len; ++ui) {
        idx_t part = ulam[ui];
        for (idx_t i = part; i <= new_max; ++i) {
            dp[i] += dp[i - part];
        }
    }

    /* Free temporary arrays. */
    free(ulam);
    free(cnt);

    /* Replace the old cache. */
    if (dp_cache) free(dp_cache);
    dp_cache = dp;
    cache_size = new_max + 1;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    /* Recompute the cache only if we have not yet covered n. */
    if (n + 1 > cache_size) {
        recompute_cache(n);
    }
    return dp_cache[n];
}
