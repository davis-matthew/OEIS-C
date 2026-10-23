/*
 * A012257  Irregular triangle read by rows: row 0 is {2}; if row n is {r_1,
 *          ..., r_k} then row n+1 is {r_k 1's, r_{k-1} 2's, r_{k-2} 3's,
 *          etc.}.
 * https://oeis.org/A012257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, nice, look
 * Formula:   a(n) is the n-th element of the irregular triangle read by rows.
 *            Row 0 = {2}; If row n = {r₁,…,r_k} then row n+1 = {r_k 1's,
 *            r_{k-1} 2's, r_{k-2} 3's, …}
 * Range:     u8 n <= 3124, u16 n <= 3124, u32 n <= 3124, u64 n <= 3124,
 *            u128 n <= 3124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* current row stored in cur_row[0 .. cur_len-1] */
    static seq_t *cur_row = NULL;
    static size_t cur_len = 0;          /* length of cur_row            */
    static idx_t  row_start = 0;        /* global index of the first
                                         element of cur_row           */

    /* initialise with first row {2} if not yet done */
    if (cur_row == NULL)
    {
        cur_row = (seq_t *)malloc(sizeof(seq_t));
        if (cur_row == NULL) abort();
        cur_row[0] = (seq_t)2;
        cur_len = 1;
        row_start = 0;
    }

    /* generate successive rows until n lies within the current one   */
    while ((idx_t)row_start + (idx_t)cur_len <= n)
    {
        size_t old_len  = cur_len;
        idx_t  old_start = row_start;

        /* compute length of the next row = sum of current row's values */
        size_t next_len = 0;
        for (size_t j = 0; j < old_len; ++j)
            next_len += (size_t)cur_row[old_len - 1 - j];

        /* allocate and build the next row */
        seq_t *next_row = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (next_row == NULL) abort();

        size_t pos = 0;
        for (size_t j = 0; j < old_len; ++j)
        {
            seq_t  count = cur_row[old_len - 1 - j];      /* how many times to repeat   */
            seq_t  value = (seq_t)(j + 1);                /* value to repeat            */
            for (size_t k = 0; k < (size_t)count; ++k)
                next_row[pos++] = value;
        }

        /* update state to new row */
        free(cur_row);
        cur_row    = next_row;
        cur_len   = next_len;
        row_start = old_start + (idx_t)old_len;
    }

    /* n is within current row: return the element at position n-row_start  */
    return cur_row[(size_t)(n - row_start)];
}
