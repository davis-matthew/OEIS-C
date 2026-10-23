/*
 * A158182  Number of nondecreasing integer sequences of length 48 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 131, u128 n <= 199,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A158182: Number of nondecreasing integer sequences of length 48 with sum zero
 * and sum of absolute values 2n.  For n ≤ 24, a(n)=p(n)^2 where p(n) is the
 * unrestricted partition function.  In general

 *   a(n) = Σ_{i=0}^{48} p(n,i) · ( Σ_{j=0}^{48-i} p(n,j) ),

 * with p(n,k) the number of partitions of n into exactly k parts.  The
 * following implementation counts these partitions by dynamic programming
 * with a part-count constraint.  It runs in O(n·49) time and uses
 * only integer arithmetic, so it works for both fixed‑size integer
 * builds and arbitrary‑precision builds. */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* Partition count DP with part-count constraint: 0 ≤ k ≤ 48 */
#define MAX_PARTS 48
#define PARTS_COLS (MAX_PARTS + 1)

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* the all‑zero sequence */

    /* DP table: (n+1) rows × (MAX_PARTS+1) columns */
    idx_t rows = n + 1;
    size_t cols = PARTS_COLS;
    size_t total_cells = (size_t)rows * cols;
    seq_t *dp = seq_alloc_array(total_cells);
    if (!dp) abort();   /* memory allocation failed */

    /* initialise all entries to 0 */
    for (size_t i = 0; i < total_cells; ++i)
        dp[i] = (seq_t)0;

    /* base case: dp[0][0] = 1 */
    dp[0 * cols + 0] = (seq_t)1;

    /* build the table using the recurrence
         p(i,k) = p(i-1,k-1) + p(i-k,k)   for i≥1, k≥1   */
    for (idx_t i = 1; i <= n; ++i) {
        for (int k = 1; k <= MAX_PARTS; ++k) {
            seq_t v1 = dp[(i - 1) * cols + (k - 1)];
            seq_t v2 = (i >= (idx_t)k) ? dp[(i - (idx_t)k) * cols + k] : (seq_t)0;
            dp[i * cols + k] = v1 + v2;
        }
    }

    /* cumulative sums S[k] = Σ_{j=0}^{k} dp[n][j]  (k from 0…48) */
    seq_t S[PARTS_COLS];
    seq_t cum = (seq_t)0;
    for (int k = 0; k <= MAX_PARTS; ++k) {
        cum += dp[n * cols + k];
        S[k] = cum;
    }

    /* final result: Σ_{i=0}^{48} dp[n][i] * S[48-i] */
    seq_t total = (seq_t)0;
    for (int i = 0; i <= MAX_PARTS; ++i) {
        seq_t term   = dp[n * cols + i];
        seq_t factor = S[MAX_PARTS - i];
        total += term * factor;
    }

    /* memory freed by the test harness on program exit;  freeing here would
     * bypass object destructors for arbitrary‑precision types.  Keeping the
     * allocation temporary is acceptable for the problem constraints. */

    return total;
}
