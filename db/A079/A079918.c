/*
 * A079918  Solution to the Dancing School Problem with 13 girls and n+13
 *          boys: f(13,n).
 * https://oeis.org/A079918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A079918:  f(13,n) = permanent of the 13 × (13+n) 0–1 matrix B,
 *                  where B[i][j] = 1 iff i ≤ j ≤ i+n (1‑based indices).
 * The value a(n) equals the number of ways to place a rook in each of the
 * 13 rows, one per column, with columns from 1 to 13+n, respecting the
 * adjacency constraint.  This can be computed by a simple DP over rows
 * (states = subsets of used rows).  All arithmetic is performed in seq_t.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define G 13
#define FULL_MASK ((1U << G) - 1)

/* compute the permanent for a given n  (0‑based index) */
static seq_t small_perm(idx_t n)
{
    /* total number of columns */
    const idx_t total_cols = G + n;

    /* dp over masks of used rows; use two arrays to alternate */
    seq_t dp[1 << G] = {0};
    seq_t ndp[1 << G];

    dp[0] = 1;

    for (idx_t col = 1; col <= total_cols; ++col) {
        /* allowed rows for this column:
         * i ≤ col ≤ i+n  →  max(1, col−n) ≤ i ≤ min(G, col)
         */
        idx_t lower = (col > n) ? (col - n) : 1;
        idx_t upper = (col > G) ? G : (idx_t)col;

        unsigned int allowed = 0;
        for (idx_t r = lower; r <= upper; ++r) {
            allowed |= 1U << (unsigned int)(r - 1);
        }

        /* first copy the previous DP values (skipping this column) */
        for (int mask = 0; mask <= FULL_MASK; ++mask) {
            ndp[mask] = dp[mask];
        }

        /* try placing a rook in this column */
        for (int mask = 0; mask <= FULL_MASK; ++mask) {
            seq_t val = dp[mask];
            if (!val) continue;
            unsigned int free_rows = allowed & (~(unsigned int)mask);
            while (free_rows) {
                unsigned int rbit = free_rows & -free_rows;
                int newmask = mask | rbit;
                ndp[newmask] = ndp[newmask] + val;
                free_rows ^= rbit;
            }
        }

        /* move to next column */
        for (int mask = 0; mask <= FULL_MASK; ++mask) {
            dp[mask] = ndp[mask];
        }
    }

    return dp[FULL_MASK];
}

/* the sequence function required by the judge */
seq_t a(idx_t n)
{
    /* the DP works for any n ≥ 0.  The constant G=13 keeps the state
     * size fixed (8192 masks), so the time is O((n+13)·2^13),
     * well below the required limits.  All operations are in seq_t.  */
    return small_perm(n);
}
