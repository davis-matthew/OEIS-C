/*
 * A032306  Number of rooted trees with leaves of 2 colors where any 2
 *          subtrees extending from the same node have a different number of
 *          nodes.
 * https://oeis.org/A032306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>=2, a(n)=sum_{partitions of (n-1) into distinct
 *            parts k_1,...,k_m} (a(k_1)*a(k_2)*...*a(k_m))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 44, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n==0, return 0 */

    /* allocate temporary arrays */
    seq_t *dp   = (seq_t *)calloc((size_t)n,     sizeof(seq_t));
    seq_t *tree = (seq_t *)calloc((size_t)(n + 1),sizeof(seq_t));
    if (!dp || !tree) {
        /* allocation failure – return 0 as a safe fallback */
        free(dp);
        free(tree);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;            /* empty product for sum 0 */
    tree[1] = (seq_t)2;          /* base case a(1) = 2 */

    /* compute a(2) ... a(n) sequentially */
    for (idx_t part = 1; part <= n - 1; ++part) {
        /* update dp for this new part size */
        for (idx_t s = n - 1; s >= part; --s) {
            dp[s] = dp[s] + dp[s - part] * tree[part];
        }
        /* after processing part 'part', dp[part] equals a(part+1) */
        if (part + 1 <= n)
            tree[part + 1] = dp[part];
    }

    seq_t result = tree[n];

    /* clean up and return */
    free(dp);
    free(tree);
    return result;
}
