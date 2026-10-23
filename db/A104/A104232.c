/*
 * A104232  Triangle read by rows, based on the morphism f: 1->2, 2->3,
 *          3->{2,1}. First row is 1. If current row is a,b,c,..., then the
 *          next row is a,b,c,...,f(a),f(b),f(c),...
 * https://oeis.org/A104232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the triangle read from rows generated
 *            by the morphism f(1) = {2}, f(2) = {3}, f(3) = {2,1}, where each
 *            next row consists of the previous row followed by the image of
 *            each element of the previous row. a(0) is the first element of
 *            the first row
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   seq_alloc, seq_free, seq_resize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, realloc */
#include <stdint.h>   /* uint64_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"
#include "../utils/seq_resize.h"

seq_t a(idx_t n)
{
    static seq_t *seq    = NULL;   /* flattened sequence */
    static size_t seq_cap = 0;
    static idx_t seq_len  = 0;

    static seq_t *row    = NULL;   /* last row */
    static size_t row_len = 0;

    if (seq == NULL) {
        seq_cap = 1;
        seq     = seq_alloc(seq_cap);
        seq[0]  = (seq_t)1;
        seq_len = 1;

        row_len = 1;
        row     = seq_alloc(row_len);
        row[0]  = (seq_t)1;
    }

    /* Extend the sequence until it contains at least n+1 terms. */
    while ((idx_t)n >= seq_len) {
        /* Count occurrences of each symbol in the current row. */
        idx_t cnt1 = 0, cnt2 = 0, cnt3 = 0;
        for (size_t i = 0; i < row_len; ++i) {
            seq_t v = row[i];
            if (v == (seq_t)1)      ++cnt1;
            else if (v == (seq_t)2) ++cnt2;
            else                    ++cnt3;   /* v == 3 */
        }

        /* Length of the new row = old row + all images appended. */
        size_t appended = (size_t)cnt1 + (size_t)cnt2 + 2 * (size_t)cnt3;
        size_t newrow_len = row_len + appended;
        seq_t *newrow = seq_alloc(newrow_len);

        /* Copy the old row. */
        for (size_t i = 0; i < row_len; ++i) newrow[i] = row[i];

        /* Append the images of each element in order. */
        size_t pos = row_len;
        for (size_t i = 0; i < row_len; ++i) {
            seq_t v = row[i];
            if      (v == (seq_t)1) newrow[pos++] = (seq_t)2;
            else if (v == (seq_t)2) newrow[pos++] = (seq_t)3;
            else { /* v == 3 */
                newrow[pos++] = (seq_t)2;
                newrow[pos++] = (seq_t)1;
            }
        }

        /* Append the new row to the flattened sequence. */
        seq_resize(&seq, &seq_cap, seq_len + newrow_len);
        for (size_t i = 0; i < newrow_len; ++i)
            seq[seq_len++] = newrow[i];

        /* Replace the old row. */
        seq_free(row, row_len);
        row     = newrow;
        row_len = newrow_len;
    }

    return seq[(size_t)n];
}
