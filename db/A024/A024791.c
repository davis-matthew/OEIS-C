/*
 * A024791  Number of 7's in all partitions of n.
 * https://oeis.org/A024791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7's appearing in all partitions of n.
 *            Implementation uses dynamic programming on the generating
 *            function x^7/(1 - x^7) * Π_{k≥1} 1/(1 - x^k). Results are cached
 *            for increasing n; the table is recomputed from scratch when a
 *            larger n is requested
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 129, u64 n <= 406,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static idx_t cap = 0;        /* maximum n currently computed */
static seq_t *dp   = NULL;   /* dp[s] = number of partitions of s */
static seq_t *dp7  = NULL;   /* dp7[s] = total number of 7's in partitions of s */

/* Recompute the tables up to n (inclusive).  The computation is done
   from scratch; the old tables are freed. */
static void recompute(idx_t n)
{
    /* Allocate new arrays of length n+1 */
    size_t N = (size_t)n;
    seq_t *new_dp  = alloc_seq_array(N + 1);
    seq_t *new_dp7 = alloc_seq_array(N + 1);

    /* Initialise: dp[0] = 1, all other entries 0 */
    new_dp[0]  = (seq_t)1;
    new_dp7[0] = (seq_t)0;
    for (size_t i = 1; i <= N; ++i)
    {
        new_dp[i]  = (seq_t)0;
        new_dp7[i] = (seq_t)0;
    }

    /* Dynamic programming */
    for (idx_t k = 1; k <= n; ++k)            /* part size */
    {
        for (idx_t s = k; s <= n; ++s)        /* current sum */
        {
            new_dp[s]  += new_dp[s - k];
            new_dp7[s] += new_dp7[s - k];
            if (k == 7)                       /* each added 7 contributes one to the count */
                new_dp7[s] += new_dp[s - k];
        }
    }

    /* Replace old tables */
    if (dp   != NULL) free_seq_array(dp);
    if (dp7  != NULL) free_seq_array(dp7);
    dp   = new_dp;
    dp7  = new_dp7;
    cap  = n;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* safety for 0‑based calls */
    if (n > cap) recompute(n);             /* grow capacity lazily */
    return dp7[n];
}
