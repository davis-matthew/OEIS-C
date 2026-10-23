/*
 * A103684  Triangle read by rows, based on the morphism f: 1->{1,2},
 *          2->{1,3}, 3->{1}. First row is 1. If current row is a,b,c,...,
 *          then the next row is a,b,c,...,f(a),f(b),f(c),...
 * https://oeis.org/A103684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 7415, u16 n <= 7415, u32 n <= 7415, u64 n <= 7415,
 *            u128 n <= 7415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Morphic construction for A103684
   a(n) = the n-th element of the triangular array
          obtained by starting with [1] and repeatedly
          appending the images f(1)=[1,2], f(2)=[1,3], f(3)=[1]
          to the current row.  The sequence is the concatenation
          of all rows.  Values: 1,1,1,2,1,1,2,1,2,1,2,1,3,...
   The function builds successive rows until it has produced at
   least n terms, caching all terms so subsequent calls are
   O(1).  The algorithm is O(n) in the number of terms generated. */
#include <stdlib.h>

static seq_t *seq_buf = NULL;   /* global sequence built so far   */
static size_t seq_len = 0;      /* number of elements in seq_buf  */
static size_t seq_cap = 0;      /* capacity of seq_buf             */

static seq_t *row_buf = NULL;   /* current row to be expanded     */
static size_t row_len = 0;      /* length of current row          */
static size_t row_cap = 0;      /* capacity of row_buf            */

/* Ensure seq_buf has space for at least "need" more elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_len + need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (seq_len + need > new_cap)
        new_cap *= 2;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure row_buf has space for at least "need" more elements. */
static void ensure_row_cap(size_t need)
{
    if (row_len + need <= row_cap)
        return;
    size_t new_cap = row_cap ? row_cap * 2 : 1;
    while (row_len + need > new_cap)
        new_cap *= 2;
    row_buf = (seq_t *)realloc(row_buf, new_cap * sizeof(seq_t));
    row_cap = new_cap;
}

/* Build the sequence until the n‑th term is available. */
static void build_upto(idx_t n)
{
    if (n == (idx_t)0)        /* safety: offset is 1 */
        return;

    if (seq_len >= (size_t)n)
        return;              /* already built */

    /* initialise first row if necessary */
    if (row_len == 0) {
        ensure_row_cap(1);
        row_buf[0] = (seq_t)1;
        row_len = 1;
        ensure_seq_cap(1);
        seq_buf[0] = (seq_t)1;
        seq_len = 1;
    }

    /* generate rows until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* 1. append the current row to the global sequence */
        for (size_t i = 0; i < row_len; ++i) {
            ensure_seq_cap(1);
            seq_buf[seq_len++] = row_buf[i];
        }

        /* 2. compute images of the current row */
        size_t img_len = 0;
        for (size_t i = 0; i < row_len; ++i) {
            seq_t v = row_buf[i];
            img_len += (v == (seq_t)1 || v == (seq_t)2) ? 2 : 1;
        }

        /* 3. append those images to the global sequence */
        ensure_seq_cap(img_len);
        for (size_t i = 0; i < row_len; ++i) {
            seq_t v = row_buf[i];
            if (v == (seq_t)1) {                /* f(1) = [1,2] */
                seq_buf[seq_len++] = (seq_t)1;
                seq_buf[seq_len++] = (seq_t)2;
            } else if (v == (seq_t)2) {          /* f(2) = [1,3] */
                seq_buf[seq_len++] = (seq_t)1;
                seq_buf[seq_len++] = (seq_t)3;
            } else {                             /* v == 3, f(3) = [1] */
                seq_buf[seq_len++] = (seq_t)1;
            }
        }

        /* 4. build the new row (old row + images) for the next iteration */
        size_t new_len = row_len + img_len;
        ensure_row_cap(new_len);
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));

        /* copy old row */
        for (size_t i = 0; i < row_len; ++i)
            new_row[i] = row_buf[i];

        /* append images to new row and also to seq_buf already done above */
        size_t pos = row_len;
        for (size_t i = 0; i < row_len; ++i) {
            seq_t v = row_buf[i];
            if (v == (seq_t)1) {
                new_row[pos++] = (seq_t)1;
                new_row[pos++] = (seq_t)2;
            } else if (v == (seq_t)2) {
                new_row[pos++] = (seq_t)1;
                new_row[pos++] = (seq_t)3;
            } else {
                new_row[pos++] = (seq_t)1;
            }
        }

        /* replace old row with new row */
        free(row_buf);
        row_buf = new_row;
        row_len = new_len;
        row_cap = new_len;
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    build_upto(n);
    if (n == (idx_t)0)        /* safety: should not happen */
        return (seq_t)0;
    return seq_buf[n - 1];
}
