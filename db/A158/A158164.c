/*
 * A158164  Number of nondecreasing integer sequences of length 30 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs of partitions of n ** into at
 *            most 30 parts, where the two parts together use at ** most 30
 *            parts in total. For n ≤ 15 this equals [p(n)]² ** where p(n) is
 *            the partition number. See the OEIS entry ** A158164 for details
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 152, u128 n <= 238,
 *            big any n
 * Helpers:   dp_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function assumes the following types are defined by the
 ** build environment:
 **   - idx_t : an unsigned integer type sufficient for indices
 **   - seq_t : an unsigned integer type or arbitrary-precision
 **            integer class that supports +, *, etc.
 */

#include <stdlib.h>
#include "../utils/dp_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1 */

    /* Allocate DP table. */
    seq_t *dp = dp_array(n);
    if (!dp) return (seq_t)0;              /* allocation failure */

    size_t rows = 31ULL;                    /* columns: parts 0..30 */

    /* Helper macro to index dp[i][k]. */
#define DP(i,k) dp[((size_t)(i))*rows + (k)]

    /* Base cases. */
    DP(0,0) = (seq_t)1;
    for (idx_t k = 1; k < rows; ++k) DP(0,k) = (seq_t)0;

    /* Fill DP table. */
    for (idx_t i = 1; i <= n; ++i) {
        DP(i,0) = (seq_t)0;                /* no partition of i into 0 parts */
        for (idx_t k = 1; k < rows && k <= i; ++k) {
            seq_t val = DP(i-1, k-1);      /* add 1 to a part */
            if (k <= i) val += DP(i-k, k);/* increase each part by 1 */
            DP(i,k) = val;
        }
    }

    /* Compute T = sum_{k=1..min(30,n)} p(n,k). */
    seq_t T = (seq_t)0;
    idx_t max_k = n < 30 ? n : 30;
    for (idx_t k = 1; k <= max_k; ++k) T += DP(n,k);

    /* Compute a(n) = sum_{k=1}^{max_k} sum_{l=1}^{max_k}
     *                such that k+l ≤ 30 of p(n,k)*p(n,l). */
    seq_t res = (seq_t)0;
    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t pk = DP(n,k);
        if (pk == (seq_t)0) continue;
        idx_t lmax = (k <= 30 - 1) ? (30 - k) : 0;
        if (lmax > max_k) lmax = max_k;
        for (idx_t l = 1; l <= lmax; ++l) {
            seq_t pl = DP(n,l);
            if (pl == (seq_t)0) continue;
            res += pk * pl;
        }
    }

    free(dp);
    return res;
}
