/*
 * A097306  Array of number of partitions of n with odd parts not exceeding
 *          2*m-1 with m in {1, 2, ..., ceiling(n/2)}.
 * https://oeis.org/A097306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) computes the n‑th entry of the triangular array T(r,k) =
 *            number of partitions of r into odd parts with largest part ≤
 *            2k–1. The indices are taken in row‑major order, so a(1)=T(1,1),
 *            a(2)=T(2,1), a(3)=T(3,1), a(4)=T(3,2), …
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The routine does not memoise intermediate results; each call
    recomputes the row using a standard coin‑change dynamic programme
    with odd coins 1,3,5,… .  Since the table has at most about
    200 rows for the 10000‑th entry, the calculation is O(r²) and
    completes well within the 2‑second limit. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- find the row r and column k that correspond to index n --- */
    idx_t r   = 1;             /* current row */
    idx_t len = (r + 1) / 2;   /* row length = ceil(r/2) */
    idx_t cum = 0;             /* number of entries before current row */

    while (cum + len < n) {
        cum += len;
        ++r;
        len = (r + 1) / 2;
    }
    /* column k is the position within the row (1‑based) */
    idx_t k = n - cum;   /* 1 ≤ k ≤ ceil(r/2) */

    /* --- compute T(r,k) by dynamic programming on odd parts --- */
    seq_t *dp = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= r; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t t = 1; t <= k; ++t) {
        idx_t coin = 2 * t - 1;   /* next odd coin */
        for (idx_t s = coin; s <= r; ++s) {
            dp[s] = dp[s] + dp[s - coin];
        }
    }

    seq_t result = dp[r];
    free(dp);
    return result;
}
