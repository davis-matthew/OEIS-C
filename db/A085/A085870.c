/*
 * A085870  Array defined by T(i,1) = 2^i-1, T(1,j) = 2^j-1, T(i,j) = T(i-1,j)
 *          + T(i-1,j-1) read by antidiagonals upwards.
 * https://oeis.org/A085870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) comes from the array T(i,j) defined by T(i,1)=2^i-1,
 *            T(1,j)=2^j-1, T(i,j)=T(i-1,j)+T(i-1,j-1) a(n) is the nth element
 *            of T read by antidiagonals upwards
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find the diagonal k (size k) that contains the n‑th term */
    idx_t k = 1;
    idx_t cum = 1;                 /* cumulative count up to diag k */
    while (cum < n) {
        ++k;
        cum += k;
    }
    idx_t cumPrev = cum - k;        /* count up to previous diagonal */
    idx_t d       = n - cumPrev - 1;/* offset within current diagonal */
    idx_t i = k - d;                /* row index (1‑based)   */
    idx_t j = 1 + d;                /* column index (1‑based) */

    /* compute the value T(i,j) */
    if (i == 1) {
        return pow2(j) - (seq_t)1;
    } else {
        size_t J = (size_t)j;
        seq_t *prev = (seq_t *)malloc((J + 1) * sizeof(seq_t));
        seq_t *cur  = (seq_t *)malloc((J + 1) * sizeof(seq_t));

        if (!prev || !cur) {
            /* allocation failure – abort with zero (should not occur) */
            free(prev);
            free(cur);
            return (seq_t)0;
        }

        /* initialise first row (i = 1) */
        for (size_t col = 1; col <= J; ++col)
            prev[col] = pow2(col) - (seq_t)1;

        /* generate rows 2 … i */
        for (idx_t row = 2; row <= i; ++row) {
            cur[1] = pow2((idx_t)row) - (seq_t)1; /* T(row,1) */
            for (size_t col = 2; col <= J; ++col)
                cur[col] = prev[col] + prev[col - 1];
            /* swap prev and cur for next iteration */
            seq_t *tmp = prev;
            prev = cur;
            cur  = tmp;
        }

        seq_t result = prev[j];
        free(prev);
        free(cur);
        return result;
    }
}
