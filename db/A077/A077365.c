/*
 * A077365  Sum of products of factorials of parts in all partitions of n.
 * https://oeis.org/A077365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/Product_{m>0} (1 - m! x^m)
 *            Recurrence: a(0)=1; for n>0 a(n) = Sum_{m=1..n} (m!) * a(n-m)
 *            where each a(k) already contains contributions from all part
 *            sizes ≤ m-1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    /* Allocate DP array for the current n.
       All entries are 0 except a[0] = 1. */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (dp == NULL) return 0;               /* memory failure unlikely */
    dp[0] = 1;                              /* a(0) = 1 */

    seq_t fact = 1;                         /* m! accumulator */

    /* Iterate over part sizes m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m) {
        fact = fact * (seq_t)m;             /* compute m! incrementally */

        /* Unbounded use of part size m:
           add its contribution to all positions ≥ m. */
        for (idx_t j = m; j <= n; ++j) {
            dp[j] = dp[j] + dp[j - m] * fact;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
