/*
 * A169975  Expansion of Product_{i>=0} (1 + x^(4*i+1)).
 * https://oeis.org/A169975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 347, u32 n <= 1012, u64 n <= 3278,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169975: number of partitions into distinct parts of the form 4*k+1.
   The function uses a 0‑1 knapsack dynamic programme that is recomputed
   from scratch whenever the requested index exceeds the largest value
   already computed.  The table dp[s] holds the number of partitions of
   s using only parts ≡ 1 (mod 4).  Because the number of parts ≤ n is
   n/4, the total cost for a maximum n of 10000 is about 12.5 million
   elementary operations, well below the time limit. */
#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *dp = NULL;          /* dp[s] = partitions of s */
    static idx_t max_n = 0;           /* largest index that has been
                                         fully computed */

    /* initialise on first use */
    if (dp == NULL) {
        dp = (seq_t *)realloc(NULL, sizeof(seq_t));
        if (!dp) abort();
        dp[0] = 1;    /* empty partition */
    }

    /* If we already have the answer, return it immediately. */
    if (n <= max_n)
        return dp[(size_t)n];

    /* Grow the array to accommodate the new maximum index. */
    size_t new_len = (size_t)(n + 1);
    dp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
    if (!dp) abort();

    /* Initialise new positions and reset the table. */
    for (idx_t i = 0; i <= n; ++i)
        dp[(size_t)i] = 0;
    dp[0] = 1;

    /* 0‑1 knapsack: iterate over all parts 1,5,9,… ≤ n. */
    for (idx_t part = 1; part <= n; part += 4) {
        for (idx_t sum = n; sum >= part; --sum) {
            dp[(size_t)sum] += dp[(size_t)(sum - part)];
        }
    }

    max_n = n;
    return dp[(size_t)n];
}
