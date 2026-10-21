/*
 * A239495  Number of (4,1)-separable partitions of n; see Comments.
 * https://oeis.org/A239495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts ≥5 with part 8
 *            excluded
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the fact that a(4,1)-separable partition of n corresponds
   bijectively to a partition of n into integers ≥5 (none equal to 8).          */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include <stdbool.h>

/* Computes the entry a(n) of OEIS sequence A239495.  */
seq_t a(idx_t n)
{
    /* The DP array is built once and reused on subsequent calls. */
    static bool          initialized = false;
    static idx_t         max_n       = 0;
    static seq_t*        dp          = NULL;

    if (n == 0)                /* the OEIS offset is 1; return 0 for n=0 */
        return 0;

    /* Build the DP up to the requested index if necessary. */
    if (!initialized || n > max_n)
    {
        idx_t new_max = n;

        /* Allocate space for dp[0..new_max] (sequence entries). */
        seq_t* new_dp = seq_alloc_new_array(new_max + 1);
        if (!new_dp)
            return 0;          /* allocation failed – just return 0 */

        /* Initialise the array.  dp[0] = 1 for the empty partition. */
        new_dp[0] = 1;
        for (idx_t i = 1; i <= new_max; ++i)
            new_dp[i] = 0;

        /* Standard DP for unrestricted partitions with allowed part sizes
           5,6,7,9,10,11,... (i.e. all ≥5 except 8).  */
        for (idx_t s = 5; s <= new_max; ++s)
        {
            if (s == 8)           /* part 8 is forbidden */
                continue;
            for (idx_t i = s; i <= new_max; ++i)
                new_dp[i] += new_dp[i - s];
        }

        /* Free the previous table if it exists. */
        if (initialized && dp)
            seq_free_array(dp);

        dp        = new_dp;
        max_n     = new_max;
        initialized = true;
    }

    return dp[n];
}
