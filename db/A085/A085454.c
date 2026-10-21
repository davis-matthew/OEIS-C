/*
 * A085454  Array defined by T(i,0) = i, T(0,j) = 2*j, T(i,j) = T(i-1,j) +
 *          T(i-1,j-1) read by antidiagonals.
 * https://oeis.org/A085454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   diagonal_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(i,j)
 * where T(0,0)=0,
 *       T(i,0)=i,
 *       T(0,j)=2*j,
 *       T(i,j)=T(i-1,j)+T(i-1,j-1)
 * The sequence a is obtained by reading T(i,j) in antidiagonal order
 * (i decreases from s to 0 on diagonal s=i+j).  The code derives the
 * coordinate (i,j) from n and then builds the triangular table up to
 * that row using the recurrence.
 */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/diagonal_of.h"

seq_t a(idx_t n)
{
    /* 1. Locate diagonal s and coordinate (i,j)  */
    idx_t s = diagonal_of(n);                    /* diagonal index */
    idx_t prev_count = (s * (s + 1)) / 2;        /* elements before diagonal s */
    idx_t offset = n - prev_count;               /* inner offset on the diagonal */
    idx_t i = s - offset;                        /* row  */
    idx_t j = offset;                            /* column */

    /* 2. Width needed for all rows up to s (columns 0..s)  */
    idx_t width = s + 1;

    /* 3. Allocate two rows for dynamic programming  */
    seq_t *prev = (seq_t *)malloc(width * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(width * sizeof(seq_t));
    if (!prev || !curr) {
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0; /* out of memory – unlikely in test bounds */
    }

    /* 4. Initialize row 0  */
    prev[0] = (seq_t)0;
    for (idx_t k = 1; k < width; ++k)
        prev[k] = (seq_t)(2 * k);

    /* 5. If the target is in row 0, return directly  */
    if (i == 0) {
        seq_t res = prev[j];
        free(prev);
        free(curr);
        return res;
    }

    /* 6. Compute rows 1..i, storing only the current and previous row  */
    for (idx_t row = 1; row <= i; ++row) {
        curr[0] = (seq_t)row;                     /* boundary: T(row,0)=row */
        for (idx_t col = 1; col < width; ++col)
            curr[col] = prev[col] + prev[col - 1]; /* recurrence  */
        /* swap prev and curr  */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* 7. Retrieve the requested element  */
    seq_t result = prev[j];

    /* 8. Clean up and return  */
    free(prev);
    free(curr);
    return result;
}
