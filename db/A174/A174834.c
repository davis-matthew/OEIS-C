/*
 * A174834  Triangle read by rows: T(n,k) = (Stirling1(n,k) +
 *          Stirling1(n,n-k)) * binomial(n,k) with T(0,0)=1.
 * https://oeis.org/A174834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   T(n,k) = (Stirling1(n,k)+Stirling1(n,n-k))·binomial(n,k) a(n) is
 *            the triangle read by rows, with a(0)=1
 * Range:     i8 n <= 11, i16 n <= 29, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special–case the seed */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Find row r and column k of the flattened triangle */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate two rows for Stirling numbers of the first kind */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;          /* allocation failure – return 0 */
    }

    /* Base row n = 0 */
    prev[0] = (seq_t)1;

    /* Build rows incrementally up to row r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)0;
        for (idx_t j = 1; j < i; ++j) {
            curr[j] = prev[j - 1] - (seq_t)(i - 1) * prev[j];
        }
        curr[i] = (seq_t)1;      /* s(i,i)=1 */

        /* swap rows */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Now prev holds the desired row r */
    seq_t s1   = prev[k];
    seq_t s2   = prev[r - k];
    seq_t sum  = s1 + s2;
    seq_t bin  = binomial(r, k);
    seq_t result = sum * bin;

    free(prev);
    free(curr);
    return result;
}
