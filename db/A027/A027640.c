/*
 * A027640  Poincaré series [or Poincare series] for ring of modular forms of
 *          genus 2.
 * https://oeis.org/A027640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 811, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p_{4,6,10,12}(n) + p_{4,6,10,12}(n-35)
 * where p_{4,6,10,12}(m) is the number of partitions of m
 * into parts 4, 6, 10 and 12 (un‑ordered).
 */

#include <stdlib.h>

static seq_t *dp     = NULL;   /* dp[k] = number of partitions of k using 4,6,10,12 */
static idx_t dp_cap  = 0;      /* allocated capacity of dp   */

/* Ensure that dp[0..max] is initialized for all sums up to max. */
static void ensure_dp(idx_t max)
{
    if (dp_cap < max + 1) {
        /* reallocate to new capacity */
        idx_t new_cap = max + 1;
        seq_t *new_dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (dp != NULL) {
            /* copy old data up to current capacity */
            idx_t copy_cap = dp_cap < new_cap ? dp_cap : new_cap;
            for (idx_t i = 0; i < copy_cap; ++i) new_dp[i] = dp[i];
            /* zero the rest */
            for (idx_t i = copy_cap; i < new_cap; ++i) new_dp[i] = 0;
            free(dp);
        } else {
            for (idx_t i = 0; i < new_cap; ++i) new_dp[i] = 0;
        }
        dp     = new_dp;
        dp_cap = new_cap;
    }

    /* recompute all values up to dp_cap-1 */
    dp[0] = 1;                 /* empty partition */
    const idx_t parts[4] = {4, 6, 10, 12};

    /* clear counts for sums > 0 */
    for (idx_t i = 1; i < dp_cap; ++i) dp[i] = 0;

    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t sum = part; sum < dp_cap; ++sum)
            dp[sum] += dp[sum - part];
    }
}

seq_t a(idx_t n)
{
    /* need maximal sum n and possibly n-35, but ensure_dp can be called
       with the larger of them */
    idx_t max_needed = n;
    if (n >= 35 && (n - 35) > max_needed) max_needed = n - 35;
    ensure_dp(max_needed);

    seq_t result = dp[n];
    if (n >= 35) result += dp[n - 35];
    return result;
}
