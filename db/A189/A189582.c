/*
 * A189582  Number of permutations of 1..n with displacements restricted to
 *          {-4,-1,0,2,3}.
 * https://oeis.org/A189582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189582: Number of permutations of 1..n with displacements
   restricted to {-4,-1,0,2,3}.  Computed via a transfer‑matrix DP over a
   sliding window of width 9 (rows i = j-4 … j+4).  The state is a 9‑bit
   mask indicating which of these rows have not yet been matched.  Only
   one row per column is matched.  No memoisation of intermediate results
   beyond the current column is required.  Complexity is O(n) with a
   constant factor 512 * 5 operations per step. */

#include <stdbool.h>   /* for bool, not used but required by spec */

#define MASK_SIZE 512   /* 2^9 */

seq_t a(idx_t n)
{
    /* States for current and next column */
    seq_t dp[MASK_SIZE] = {0};
    seq_t ndp[MASK_SIZE] = {0};

    /* Initial mask for column j = 1 */
    int init = 0;
    {
        /* Row indices 1..min(5,n) are present.
           For j = 1, window start = j-4 = -3.
           Bit index = i - (j-4) = i + 3. */
        idx_t upto = n < 5 ? n : 5;
        for (idx_t i = 1; i <= upto; ++i)
            init |= 1 << ((int)(i + 3));
    }
    dp[init] = 1;

    /* Allowed bits (relative to window) for displacement set {-4,-1,0,2,3} */
    static const int allowed[5] = {1, 2, 4, 5, 8};

    for (idx_t pos = 1; pos <= n; ++pos)
    {
        /* reset next dp array */
        for (int i = 0; i < MASK_SIZE; ++i)
            ndp[i] = 0;

        for (int mask = 0; mask < MASK_SIZE; ++mask)
        {
            seq_t ways = dp[mask];
            if (!ways) continue;

            /* Bit 0 represents row pos-4; it must already be matched */
            if (mask & 1) continue;

            /* For each available row that can match current column */
            for (int k = 0; k < 5; ++k)
            {
                int b = allowed[k];
                if (mask & (1 << b))
                {
                    int new_mask = mask & ~(1 << b);   /* mark row matched */
                    int next_mask = new_mask >> 1;     /* shift window */
                    /* Add new row pos+5 if it exists */
                    if (pos + 5 <= n)
                        next_mask |= (1 << 8);
                    ndp[next_mask] = ndp[next_mask] + ways;
                }
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < MASK_SIZE; ++i)
            dp[i] = ndp[i];
    }

    /* After processing all columns, all rows must be matched => mask == 0 */
    return dp[0];
}
