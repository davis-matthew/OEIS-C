/*
 * A161114  Number of partitions of n into nonzero triangular numbers where
 *          every part appears at least 13 times.
 * https://oeis.org/A161114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of partitions of n into non‑zero triangular
 *            numbers * where each part appears at least 13 times. *
 *            Computation: dynamic programming over all triangular numbers t ≤
 *            n. * For each t we add partitions that use t at least 13 times;
 *            extra copies * of t (≥0) are handled with an unboun...
 * Range:     u8 n <= 200, u16 n <= 568, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Internal cache used for dynamic programming arrays.
 * Size is increased as needed between calls. */
static seq_t *dp = NULL;          /* partitions ignoring the current triangle */
static seq_t *dp_use = NULL;      /* partitions using the current triangle */
static size_t dp_cap = 0;         /* allocated capacity of dp and dp_use */

/* Ensure the internal arrays can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;  /* number of entries required */
    if (dp_cap >= needed)
        return;

    /* Allocate new larger arrays */
    seq_t *new_dp;
    seq_t *new_dp_use;
#ifdef __cplusplus
    new_dp      = new seq_t[needed];
    new_dp_use  = new seq_t[needed];
#else
    new_dp      = (seq_t*)malloc(needed * sizeof(seq_t));
    new_dp_use  = (seq_t*)malloc(needed * sizeof(seq_t));
#endif

    /* Copy existing data if any */
    if (dp != NULL) {
        for (size_t i = 0; i < dp_cap; ++i) {
            new_dp[i]      = dp[i];
            new_dp_use[i]  = dp_use[i];
        }
#ifdef __cplusplus
        delete[] dp;
        delete[] dp_use;
#else
        free(dp);
        free(dp_use);
#endif
    }

    dp = new_dp;
    dp_use = new_dp_use;
    dp_cap = needed;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n < 13 the answer is 0 (no triangular number can appear
     * at least 13 times). */
    if (n < 13)
        return (seq_t)0;

    /* Ensure sufficient storage for this n. */
    ensure_capacity(n);

    /* Initialise dp: only the empty partition exists. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Enumerate triangular numbers t = k(k+1)/2 <= n. */
    for (idx_t k = 1; ; ++k)
    {
        idx_t t = k * (k + 1) / 2;
        if (t > n)
            break;

        /* Reset dp_use array. */
        for (idx_t i = 0; i <= n; ++i)
            dp_use[i] = (seq_t)0;

        /* Add base case: exactly 13 copies of t. */
        idx_t min_copies = 13;
        idx_t base = min_copies * t;
        if (base <= n) {
            for (idx_t s = base; s <= n; ++s)
                dp_use[s] = dp[s - base];
        }

        /* Add extra copies of t (unbounded).  Only partitions already
         * containing t at least once are considered. */
        for (idx_t s = t; s <= n; ++s)
            dp_use[s] += dp_use[s - t];

        /* Merge back into dp. */
        for (idx_t s = 0; s <= n; ++s)
            dp[s] += dp_use[s];
    }

    return dp[n];
}
