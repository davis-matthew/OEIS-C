/*
 * A208699  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A208699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   triple_forbidden
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208699 – Number of 4×n 0..1 arrays that avoid the horizontal patterns
 *          0 0 1 and 1 0 0, and that avoid the vertical patterns
 *          0 1 0 and 1 0 1.
 *
 *  The sequence is computed by dynamic programming over the 10 admissible
 *  column patterns (those that already satisfy the vertical constraints).
 *  For each step we keep a table dp[p][q] that counts the number of arrays
 *  ending with the last two columns equal to patterns p and q.
 *  Transition to a new column r is allowed iff for every row the triple
 *  (p_i, q_i, r_i) is not 001 or 100.  The first two columns are
 *  arbitrary admissible patterns.
 *  The implementation precomputes a compatibility table once (on the
 *  first call) and then performs an O(n) DP.
 */

#include <stdbool.h>
#include "../utils/triple_forbidden.h"

/* List of the 10 admissible column patterns (4 bits). */
static const unsigned pattern[10] = {0,1,3,6,7,8,9,12,14,15};

/* Precompute compatibility table: allow[p][q][r] is true iff
 * pattern p, q, r can follow consecutively with no forbidden
 * horizontal triple.  The table is computed once and reused. */
static void compute_compat(bool allow[10][10][10])
{
    for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 10; ++j)
    for (int k = 0; k < 10; ++k) {
        bool ok = true;
        for (int row = 0; row < 4 && ok; ++row) {
            unsigned a = (pattern[i] >> row) & 1U;
            unsigned b = (pattern[j] >> row) & 1U;
            unsigned c = (pattern[k] >> row) & 1U;
            if (triple_forbidden(a, b, c))
                ok = false;
        }
        allow[i][j][k] = ok;
    }
}

seq_t a(idx_t n)
{
    /* Return 0 for index 0 – behaviour below offset does not matter. */
    if (n == 0) return (seq_t)0;

    /* Base cases. */
    if (n == 1) return (seq_t)10;      /* 10 admissible columns. */
    if (n == 2) return (seq_t)100;     /* 10 × 10 pairs. */

    /* Lazily initialise the compatibility table once. */
    static bool allow[10][10][10];
    static bool init = false;
    if (!init) {
        compute_compat(allow);
        init = true;
    }

    /* DP tables: dp[p][q] – number of arrays ending with columns (p,q). */
    seq_t dp[10][10] = {0};
    seq_t next[10][10] = {0};

    /* Initialise for length 2: each pair contributes 1. */
    for (int p = 0; p < 10; ++p)
    for (int q = 0; q < 10; ++q)
        dp[p][q] = (seq_t)1;

    /* Iterate from column 3 up to n. */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero the next table. */
        for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            next[i][j] = (seq_t)0;

        /* Transition. */
        for (int p = 0; p < 10; ++p)
        for (int q = 0; q < 10; ++q) {
            seq_t val = dp[p][q];
            if (val == (seq_t)0) continue;
            for (int r = 0; r < 10; ++r) {
                if (allow[p][q][r])
                    next[q][r] += val;
            }
        }

        /* Prepare for next iteration. */
        for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            dp[i][j] = next[i][j];
    }

    /* Sum all states for the answer. */
    seq_t sum = (seq_t)0;
    for (int p = 0; p < 10; ++p)
    for (int q = 0; q < 10; ++q)
        sum += dp[p][q];

    return sum;
}
