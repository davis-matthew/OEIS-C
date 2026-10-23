/*
 * A189588  Number of permutations of 1..n with displacements restricted to
 *          {-5,-4,-3,0,1,2}.
 * https://oeis.org/A189588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n with displacements
 *            restricted to the set {-5,-4,-3,0,1,2}. This is computed by
 *            dynamic programming on the banded adjacency matrix of allowed
 *            rook placements. The algorithm runs in O(n·2^8) time and uses at
 *            most 256 state values
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 56, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types are provided by the build system. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Guard against non‑positive indices.  The sequence is defined for n ≥ 1. */
    if (n == 0) return (seq_t)0;

    /* Offsets allowed for a single row.  Index –5 is represented by bit 0
       in the 8‑bit mask to avoid sign complications. */
    static const int offsets[6] = {-5, -4, -3, 0, 1, 2};

    /* State tables: dp[mask] holds the number of ways after processing a
       certain number of rows, where mask records which of the eight
       columns (row‑5 … row+2) are already occupied. */
    static seq_t dp[256];
    static seq_t next[256];

    /* Initialise for the first row. */
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Main DP loop over all rows 1 … n. */
    for (idx_t row = 1; row <= n; ++row)
    {
        for (int i = 0; i < 256; ++i) next[i] = (seq_t)0;

        for (int mask = 0; mask < 256; ++mask)
        {
            seq_t cur = dp[mask];
            if (cur == (seq_t)0) continue;

            /* For each allowed displacement, try to place the rook. */
            for (int k = 0; k < 6; ++k)
            {
                int d   = offsets[k];
                int col = (int)row + d;          /* column index (1‑based) */
                if (col < 1 || col > (int)n)     /* outside the board */
                    continue;

                int bit = d + 5;                 /* bit position in 8‑bit mask */
                if (mask & (1 << bit))           /* column already used */
                    continue;

                int new_mask = mask | (1 << bit);
                if (row != n)                    /* shift for next row */
                    new_mask >>= 1;

                next[new_mask] += cur;
            }
        }

        /* Prepare for the next iteration. */
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* After processing all rows, compute the mask that indicates all
       columns 1 … n are occupied.  Bits correspond to columns
       row−5 … row+2 (after the final row, row = n). */
    int needed_mask = 0;
    for (int i = 0; i < 8; ++i)
    {
        int col = (int)n - 5 + i;         /* actual column index */
        if (col >= 1 && col <= (int)n)    /* inside the board */
            needed_mask |= (1 << i);
        /* Bits for columns outside 1..n are automatically 0. */
    }

    return dp[needed_mask];
}
