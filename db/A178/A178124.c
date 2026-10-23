/*
 * A178124  Coefficient array of orthogonal polynomials
 *          P(n,x)=(x-n)*P(n-1,x)-(2n-3)*P(n-2,x), P(0,x)=1,P(1,x)=x-1.
 * https://oeis.org/A178124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient array of orthogonal polynomials P(n,x) =
 *            (x-n)*P(n-1,x)-(2n-3)*P(n-2,x), with P(0,x)=1, P(1,x)=x-1
 * Range:     i8 n <= 22, i16 n <= 45, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   free_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/free_row.h"

/* Allocate a temporary row of seq_t's.  Handles C and C++ */
static seq_t *alloc_row(size_t len)
{
    seq_t *p;
#ifdef __cplusplus
    p = new seq_t[len];
#else
    p = (seq_t*)malloc(len * sizeof(seq_t));
#endif
    if (!p) abort();
    return p;
}

/* Return the n-th term of the OEIS sequence A178124. */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n.  Row r contains r+1 terms
       and starts at index r*(r+1)/2.  We use smallest r such that
       (r+1)*(r+2)/2 > n. */
    idx_t r = 0;
    while ( ((idx_t)(r+1) * (r+2) / 2) <= n ) r++;

    idx_t start = ((idx_t)r * (r+1) / 2);   /* starting index of row r */
    idx_t k = n - start;                    /* coefficient position in row r */

    if (r == 0) return (seq_t)1;           /* only one term, a(0) */

    /* Generate rows 0, 1, …, r.  Only two previous rows are kept. */
    seq_t *prevprev = NULL;
    seq_t *prev     = NULL;
    size_t prevprev_len = 0, prev_len = 0;

    /* row 0 */
    prevprev = alloc_row(1);
    prevprev[0] = (seq_t)1;
    prevprev_len = 1;

    /* row 1 */
    prev = alloc_row(2);
    prev[0] = -(seq_t)1;
    prev[1] = (seq_t)1;
    prev_len = 2;

    if (r == 1) {
        seq_t result = prev[k];
        free_row(prev);
        free_row(prevprev);
        return result;
    }

    /* Build successive rows up to r */
    for (idx_t i = 2; i <= r; ++i) {
        size_t len = (size_t)i + 1;
        seq_t *curr = alloc_row(len);

        seq_t coeff_n     = (seq_t)i;
        seq_t coeff_2n_3  = (seq_t)(2 * (int64_t)i - 3);

        for (size_t j = 0; j < len; ++j) {
            seq_t val = 0;
            if (j > 0) val += prev[j-1];
            if (j < len-1) val -= coeff_n * prev[j];
            if (j < len-2) val -= coeff_2n_3 * prevprev[j];
            curr[j] = val;
        }

        free_row(prevprev);
        prevprev = prev;
        prevprev_len = prev_len;
        prev = curr;
        prev_len = len;
    }

    seq_t result = prev[k];
    free_row(prev);
    free_row(prevprev);
    return result;
}
