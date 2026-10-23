/*
 * A115276  Number of partitions of {1,...,n} into block sizes not a multiple
 *          of 4.
 * https://oeis.org/A115276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n, k mod 4 ≠ 0} binom(n-1, k-1) * a(n-k), a(0) =
 *            1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recursive uses a Pascal‑triangle row for binomial coefficients.
   Memory for the DP array and the binomial row is allocated with the
   helper functions that work for both native and arbitrary‑precision
   integers. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */
#include "../utils/free_seq_array.h"    /* free_seq_array */

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL;     /* dp[i] = a(i)   */
    static seq_t *C    = NULL;     /* C[j]  = binom(row, j) */
    static size_t  cap          = 0;   /* allocated length   */
    static idx_t   max_computed = 0;   /* largest n computed */

    size_t need = (size_t)n + 1;

    /* Allocate fresh arrays if none yet exist */
    if (dp == NULL) {
        dp = alloc_seq_array(need);
        C  = alloc_seq_array(need);
        for (size_t i = 0; i < need; ++i) {
            dp[i] = 0;
            C[i]  = 0;
        }
        dp[0] = 1;           /* a(0) = 1                      */
        C[0]  = 1;           /* first row of Pascal triangle   */
        cap          = need;
        max_computed = 0;
    }
    /* Grow the arrays if required */
    else if (need > cap) {
        seq_t *new_dp = alloc_seq_array(need);
        seq_t *new_C  = alloc_seq_array(need);
        for (size_t i = 0; i < need; ++i) {
            if (i < cap) {
                new_dp[i] = dp[i];
                new_C[i]  = C[i];
            } else {
                new_dp[i] = 0;
                new_C[i]  = 0;
            }
        }
        free_seq_array(dp);
        free_seq_array(C);
        dp = new_dp;
        C  = new_C;
        cap = need;
    }

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* C currently holds row (i-1) of Pascal's triangle */
        for (idx_t k = 1; k <= i; ++k) {
            if (k & 3) {                   /* k % 4 != 0           */
                sum += C[k - 1] * dp[i - k];
            }
        }
        dp[i] = sum;

        /* Update C to the next row: C[j] = C[j-1] + C[j]  (j from i down to 1) */
        for (idx_t j = i; j > 0; --j) {
            C[j] = C[j - 1] + C[j];
        }
    }
    max_computed = n;
    return dp[n];
}
