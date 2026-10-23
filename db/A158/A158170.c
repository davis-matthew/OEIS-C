/*
 * A158170  Number of nondecreasing integer sequences of length 36 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{min(36,n)} Σ_{j=0}^{min(36-k,n)} p_k(n) * p_j(n)
 *            where p_k(n) is the number of partitions of n into exactly k
 *            parts. (Equivalently, the number of nondecreasing integer
 *            sequences of length 36 with sum zero and sum of absolute values
 *            2n.)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 140, u128 n <= 222,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* dp row 0..n, column 0..36: p_k(row) */
    static seq_t *dp = NULL;      /* linear array dp[row*37 + col] */
    static idx_t max_computed = 0;/* largest n for which dp is valid */

    if (n < 1)
        return 0;

    /* expand the table if needed */
    if (n > max_computed) {
        idx_t new_max = n;
        size_t new_size = (size_t)(new_max + 1) * 37;
        seq_t *newdp = seq_alloc_array(new_size);
        if (!newdp)
            return 0; /* allocation failure */
        memset(newdp, 0, new_size * sizeof(seq_t));

        /* copy old data if any */
        if (dp) {
            size_t old_size = (size_t)(max_computed + 1) * 37;
            memcpy(newdp, dp, old_size * sizeof(seq_t));
            seq_free_array(dp);
        }
        dp = newdp;

        /* initialise base case dp[0][0] = 1 if not already set */
        if (max_computed == 0 && dp[0 * 37 + 0] == 0)
            dp[0 * 37 + 0] = 1;

        /* compute rows from max_computed+1 up to n */
        idx_t start = max_computed + 1;
        for (idx_t i = start; i <= n; ++i) {
            dp[i * 37 + 0] = 0;               /* p_0(i) = 0 for i>0 */
            idx_t max_k = (i < 36 ? i : 36);
            for (idx_t k = 1; k <= max_k; ++k) {
                seq_t val1 = dp[(i - k) * 37 + k]; /* only if i >= k */
                seq_t val2 = dp[(i - 1) * 37 + (k - 1)];
                dp[i * 37 + k] = val1 + val2;
            }
        }
        max_computed = n;
    }

    /* compute a(n) from the table */
    seq_t res = 0;
    idx_t max_k = (n < 36 ? n : 36);
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t pk = dp[n * 37 + k];
        if (pk == 0) continue;
        idx_t max_j = (36 > k ? (36 - k) : 0);
        if (max_j > n) max_j = n;
        for (idx_t j = 0; j <= max_j; ++j) {
            seq_t pj = dp[n * 37 + j];
            if (pj == 0) continue;
            res += pk * pj;
        }
    }
    return res;
}
