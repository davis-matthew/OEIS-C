/*
 * A029052  Expansion of 1/((1-x)(1-x^3)(1-x^7)(1-x^11)).
 * https://oeis.org/A029052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into the parts 1, 3, 7, and 11.
   Computed by a simple dynamic‑programming routine that
   updates a table dp[0..n] with the recurrence
       dp[0] = 1,
       dp[i] += dp[i - p]   for each part p in {1,3,7,11}  (i ≥ p).   */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp   = NULL;   /* cache for computed values */
static idx_t  size = 0;      /* current storage size */

/* Ensure that dp is large enough to hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        dp = tmp;
        size = new_size;
    }
}

seq_t a(idx_t n)
{
    const idx_t parts[4] = {1, 3, 7, 11};

    ensure_cache(n);

    /* Initialise base case and zero out the table. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Dynamic programming over the allowed parts. */
    for (unsigned k = 0; k < 4; ++k) {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}
