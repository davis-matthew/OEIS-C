/*
 * A126597  Triangle read by rows: Start with the row 1,2. To get the next
 *          row, do the following: if the sum of two adjacent terms is odd
 *          then insert this sum in between them, otherwise insert the
 *          absolute value of their difference; repeat the procedure.
 * https://oeis.org/A126597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2. 
   For each subsequent row, insert between every pair of adjacent
   terms either the sum (if the sum is odd) or the absolute difference
   (if the sum is even).  The sequence is the concatenation of all rows
   read row by row. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility: grow an array of seq_t if needed. */
static seq_t *grow_seq(seq_t *ptr, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return ptr;
    size_t new_cap = *cap ? *cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    ptr = (seq_t *)realloc(ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
    return ptr;
}

seq_t a(idx_t n)
{
    /* Statically cached data. */
    static seq_t *terms      = NULL;        /* linearized triangle terms */
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    static seq_t *row          = NULL;      /* current row */
    static seq_t *row_next     = NULL;      /* buffer for next row */
    static size_t row_len      = 0;
    static size_t row_cap      = 0;
    static size_t row_next_len = 0;
    static size_t row_next_cap = 0;

    /* ---- initialize first row and terms if this is the first call ---- */
    if (terms_len == 0) {
        /* first row: 1, 2 */
        row     = grow_seq(row,      &row_cap,      2);
        row[0] = 1;
        row[1] = 2;
        row_len = 2;

        terms   = grow_seq(terms,    &terms_cap,    2);
        terms[0] = 1;
        terms[1] = 2;
        terms_len = 2;
    }

    /* ---- generate rows until we have at least n terms ---- */
    while (terms_len < (size_t)n) {
        /* size needed for next row: 2*row_len - 1 */
        size_t next_needed = row_len * 2 - 1;
        row_next = grow_seq(row_next, &row_next_cap, next_needed);
        size_t idx = 0;

        /* copy first element */
        row_next[idx++] = row[0];

        /* process pairs */
        for (size_t i = 0; i + 1 < row_len; ++i) {
            seq_t a1 = row[i];
            seq_t a2 = row[i + 1];
            seq_t sum = a1 + a2;
            seq_t mid;
            if (sum & 1)          /* odd sum: insert the sum itself */
                mid = sum;
            else                  /* even sum: insert absolute difference */
                mid = a1 >= a2 ? a1 - a2 : a2 - a1;
            row_next[idx++] = mid;
            row_next[idx++] = a2;
        }
        row_next_len = idx;   /* should equal next_needed */

        /* append the new row into the terms array */
        terms = grow_seq(terms, &terms_cap,
                         terms_len + row_next_len);
        for (size_t i = 0; i < row_next_len; ++i)
            terms[terms_len + i] = row_next[i];
        terms_len += row_next_len;

        /* swap buffers for next iteration */
        seq_t *tmp  = row;
        row         = row_next;
        row_next    = tmp;

        size_t tmp_len = row_len;
        row_len        = row_next_len;
        row_next_len   = tmp_len;
    }

    return terms[n - 1];
}
