/*
 * A210964  Column 10 of square array A195825. Also column 1 of triangle
 *          A210954. Also 1 together with the row sums of triangle A210954.
 * https://oeis.org/A210964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 178, u32 n <= 518, u64 n <= 1678,
 *            u128 n <= 5855, big any n
 * Helpers:   seq_alloc, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A210964
   Formula (DP):  a(n) = number of partitions of n into parts congruent
   to 0, 1, or 11 modulo 12.
   We compute this by an unbounded knapsack DP with memoisation. */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_free_array.h"

/* DP cache */
static seq_t *dp = NULL;       /* dp[i] = a(i) for 0 <= i <= dp_max */
static idx_t  dp_max = 0;      /* highest index filled in dp           */

/* Build the DP table up to index N (inclusive).  dp[0] = 1. */
static void compute_up_to(idx_t N)
{
    /* Allocate or reallocate the array.  Old contents are discarded. */
    seq_t *old = dp;
    dp = seq_alloc((size_t)N + 1);
    dp_max = N;

    if (old != NULL)
        seq_free_array(old);

    /* Initialise all entries to zero, with dp[0] = 1. */
    for (idx_t i = 0; i <= N; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Unbounded knapsack over allowed part sizes. */
    for (idx_t part = 1; part <= N; ++part) {
        int r = part % 12;
        if (r == 0 || r == 1 || r == 11) {
            for (idx_t i = part; i <= N; ++i)
                dp[i] += dp[i - part];
        }
    }
}

/* Ensure that dp[n] is available; if necessary, rebuild a larger table. */
static void ensure_computed(idx_t n)
{
    if (dp == NULL) {
        /* First call – pre‑compute a modest maximum. */
        size_t limit = 10000;
        if ((idx_t)limit < n)
            while (limit < n) limit <<= 1;   /* double until large enough */
        compute_up_to((idx_t)limit);
    } else if (n > dp_max) {
        /* Requested n is beyond current table – recompute a larger table. */
        size_t limit = (size_t)dp_max;
        while (limit < n) limit <<= 1;
        compute_up_to((idx_t)limit);
    }
}

/* External linkage function required by the tests. */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return dp[n];
}
