/*
 * A035684  Number of partitions of n into parts 8k+1 and 8k+7 with at least
 *          one part of each type.
 * https://oeis.org/A035684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total_partitions(n) – partitions_using_only_1_mod8(n) –
 *            partitions_using_only_7_mod8(n) where total_partitions counts
 *            partitions of n using parts {8k+1, 8k+7} and the other two
 *            counts use only one of the two residue classes
 * Range:     u8 n <= 62, u16 n <= 159, u32 n <= 478, u64 n <= 1588,
 *            u128 n <= 5000, big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate DP arrays for total, only-1mod8, only-7mod8 partitions */
    seq_t *dp_total = alloc_seq_array((size_t)n + 1);
    seq_t *dp1_only = alloc_seq_array((size_t)n + 1);
    seq_t *dp7_only = alloc_seq_array((size_t)n + 1);
    if (!dp_total || !dp1_only || !dp7_only) {
        /* allocation failure – return 0 (unlikely in tests) */
        free_seq(dp_total); free_seq(dp1_only); free_seq(dp7_only);
        return 0;
    }

    /* Initialise to zero and base case: one way to make 0 */
    for (idx_t i = 0; i <= n; ++i) {
        dp_total[i] = dp1_only[i] = dp7_only[i] = 0;
    }
    dp_total[0] = dp1_only[0] = dp7_only[0] = 1;

    /* Process all 8k+1 parts (type 1) */
    for (idx_t k = 0;; ++k) {
        idx_t val = 8 * k + 1;
        if (val > n) break;
        for (idx_t i = val; i <= n; ++i) {
            dp_total[i] += dp_total[i - val];
            dp1_only[i]  += dp1_only[i - val];
        }
    }

    /* Process all 8k+7 parts (type 2) */
    for (idx_t k = 0;; ++k) {
        idx_t val = 8 * k + 7;
        if (val > n) break;
        for (idx_t i = val; i <= n; ++i) {
            dp_total[i] += dp_total[i - val];
            dp7_only[i]  += dp7_only[i - val];
        }
    }

    /* a(n) = dp_total[n] – dp1_only[n] – dp7_only[n] */
    seq_t result = dp_total[n];
    result -= dp1_only[n];
    result -= dp7_only[n];

    /* Free the allocated arrays */
    free_seq(dp_total);
    free_seq(dp1_only);
    free_seq(dp7_only);

    return result;
}
