/*
 * A104231  Triangle read by rows, based on the morphism f: 1->2, 2->3,
 *          3->{3,3,5,4}, 4->5, 5->6, 6->{6,6,2,1}. First row is 1. If current
 *          row is a,b,c,..., then the next row is
 *          a,b,c,...,f(a),f(b),f(c),...
 * https://oeis.org/A104231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A104231: Triangle read by rows with morphism f:
 *   1 -> 2,   2 -> 3,
 *   3 -> 3,3,5,4,
 *   4 -> 5,   5 -> 6,
 *   6 -> 6,6,2,1
 * First row is [1].  Each subsequent row is the previous row
 * followed by f applied to each element of the previous row.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of all previously generated terms */
    static seq_t *seq = NULL;    /* flat sequence of all rows */
    static size_t seq_len = 0;   /* number of terms stored */
    static size_t seq_cap = 0;   /* allocated capacity of seq */

    /* static cache for the current (last) row */
    static seq_t *row = NULL;
    static size_t row_len = 0;

    /* initialise on first call */
    if (seq == NULL) {
        seq_cap = 1;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;

        row_len = 1;
        row = (seq_t*)malloc(row_len * sizeof(seq_t));
        row[0] = (seq_t)1;
    }

    /* generate rows until we have at least n+1 terms */
    while (seq_len <= (size_t)n) {
        /* compute the number of f(v) elements that will be appended */
        size_t extra = 0;
        for (size_t i = 0; i < row_len; ++i) {
            seq_t v = row[i];
            if (v == (seq_t)1 || v == (seq_t)2 ||
                v == (seq_t)4 || v == (seq_t)5) {
                extra += 1;
            } else { /* v == 3 or v == 6 */
                extra += 4;
            }
        }
        size_t new_row_len = row_len + extra;

        /* allocate buffer for the new row */
        seq_t *new_row = (seq_t*)malloc(new_row_len * sizeof(seq_t));

        /* first copy the old row */
        for (size_t i = 0; i < row_len; ++i)
            new_row[i] = row[i];

        /* append f(v) for each element of the old row */
        size_t pos = row_len;
        for (size_t i = 0; i < row_len; ++i) {
            seq_t v = row[i];
            if (v == (seq_t)1) {
                new_row[pos++] = (seq_t)2;
            } else if (v == (seq_t)2) {
                new_row[pos++] = (seq_t)3;
            } else if (v == (seq_t)3) {
                new_row[pos++] = (seq_t)3;
                new_row[pos++] = (seq_t)3;
                new_row[pos++] = (seq_t)5;
                new_row[pos++] = (seq_t)4;
            } else if (v == (seq_t)4) {
                new_row[pos++] = (seq_t)5;
            } else if (v == (seq_t)5) {
                new_row[pos++] = (seq_t)6;
            } else { /* v == 6 */
                new_row[pos++] = (seq_t)6;
                new_row[pos++] = (seq_t)6;
                new_row[pos++] = (seq_t)2;
                new_row[pos++] = (seq_t)1;
            }
        }

        /* ensure seq has enough space for the new terms */
        if (seq_len + new_row_len > seq_cap) {
            seq_cap = seq_len + new_row_len;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }

        /* copy the new row into seq */
        for (size_t i = 0; i < new_row_len; ++i)
            seq[seq_len + i] = new_row[i];
        seq_len += new_row_len;

        /* replace the current row */
        free(row);
        row = new_row;
        row_len = new_row_len;
    }

    return seq[n];
}
