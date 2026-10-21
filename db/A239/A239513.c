/*
 * A239513  Number of partitions p of n such that if h = (number of parts of
 *          p), then h is an (h,0)-separator of p; see Comments.
 * https://oeis.org/A239513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{odd m≥3, k=(m−1)/2, t=(m+1)/2, R=n−k·m≥0} * [ P(t,R) −
 *            P(t−1,R−m) ] * where P(t,s) is the number of partitions of s
 *            into exactly t parts. * P(0,0)=1 and, for t>0,
 *            P(t,s)=P(t−1,s−1)+P(t,s−t) (s≥t)
 * Range:     u8 n <= 5, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   compute_partition_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free, size_t */
#include <string.h>   /* for memset */
#include "../utils/compute_partition_table.h"

seq_t a(idx_t n)
{
    /* determine the largest t needed:
       k*(2k+1) ≤ n  =>  t = k+1   (k = (m-1)/2)  */
    size_t max_t = 0;
    for (size_t k = 1; k * (2 * k + 1) <= n; ++k)
        max_t = k + 1;           /* t = k+1 */

    /* allocate space for table P : (max_t+1) × (n+1)  */
    size_t rows = max_t + 1;
    size_t cols = (size_t)n + 1;
    seq_t *P = (seq_t *)malloc(rows * cols * sizeof(seq_t));
    if (P == NULL)
        return 0;   /* allocation failure; return 0 as safe fallback */

    /* compute DP table */
    compute_partition_table(n, max_t, P);

    seq_t ans = 0;

    /* iterate over odd m ≥ 3 */
    for (idx_t m = 3; m <= n; m += 2) {
        size_t k = (m - 1) >> 1;          /* k = (m-1)/2 */
        size_t t = k + 1;                 /* t = (m+1)/2    */

        seq_t km = (seq_t)k * (seq_t)m;   /* k * m */
        if (km > (seq_t)n) break;         /* R would be negative */

        idx_t R = n - k * m;
        seq_t add = P[t * cols + (size_t)R];          /* P(t,R) */

        if (R >= m) {
            seq_t sub = P[(t - 1) * cols + (size_t)(R - m)];
            add -= sub;           /* P(t-1,R-m) */
        }
        ans += add;
    }

    free(P);
    return ans;
}
