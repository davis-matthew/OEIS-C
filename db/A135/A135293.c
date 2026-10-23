/*
 * A135293  Differences between successive numbers whose sum of digits in base
 *          3 is 2.
 * https://oeis.org/A135293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 76, u32 n <= 251, u64 n <= 901,
 *            u128 n <= 1274, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A135293 – differences between successive numbers
 * whose sum of digits in base 3 equals 2.
 * The triangle recurrence is used:
 *   T(0,0) = 2;
 *   T(1,0)=T(1,1)=2;
 *   For rows r ≥ 1:
 *     T(r+1,0)   = T(r,0)   + T(r,r);
 *     T(r+1,m)   = T(r,m)   for 1 ≤ m ≤ r;
 *     T(r+1,r+1) = sum_{k=0..r} T(r+1,k).
 *
 * The sequence a(n) is the flattened triangle.  For column
 * m > 0 we have the simple formula T(r,m)=2·3^{m-1}, so the only
 * non‑trivial values are the first column.
 */

#include <stdlib.h>
#include "../utils/pow3.h"          /* seq_t pow3(idx_t); */

seq_t a(idx_t n)
{
    /* handle the very first term */
    if (n == 0)
        return (seq_t)2;

    /* begin with row 1: 2 2 */
    size_t cur_len = 2;
    seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
    cur[0] = (seq_t)2;
    cur[1] = (seq_t)2;

    idx_t offset = 1;                 /* first index of the current row */

    /* generate next rows until the desired linear index falls into the
     * current row.  For each iteration old_len is the length of the
     * preceding row (r ≥ 1).  The recurrence works for all such rows. */
    while (offset + (idx_t)cur_len <= n) {
        size_t old_len = cur_len;
        seq_t *next = (seq_t *)malloc((old_len + 1) * sizeof(seq_t));

        /* compute sum of previous row */
        seq_t sum_cur = cur[0];
        for (size_t i = 1; i < old_len; ++i)
            sum_cur += cur[i];

        /* first element of new row */
        next[0] = cur[0] + cur[old_len - 1];

        /* copy the remaining columns from the previous row */
        for (size_t i = 1; i < old_len; ++i)
            next[i] = cur[i];

        /* last element of new row */
        next[old_len] = sum_cur + cur[old_len - 1];

        free(cur);
        cur = next;
        cur_len = old_len + 1;
        offset += old_len;            /* next row starts here */
    }

    /* index of the element within the current row */
    idx_t col = n - offset;

    seq_t result;
    if (col == 0)
        result = cur[0];              /* first column value */
    else
        result = (seq_t)2 * pow3(col - 1); /* 2 * 3^{col-1} */

    free(cur);
    return result;
}
