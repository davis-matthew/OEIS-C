/*
 * A131309  Irregular triangle: rows are iterates of the morphism with maps 1
 *          -> 110, 0 -> 10.
 * https://oeis.org/A131309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9747, u16 n <= 9747, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131309:  a(n) is the nth term of the infinite concatenation of rows obtained
 * from the morphism 1 → 110, 0 → 10 with axiom 1.  The sequence therefore
 * consists of 0s and 1s and begins
 * 1; 1,1,0; 1,1,0,1,1,0,1,0; ...
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding the concatenated sequence and the
       most recent morphic row used to generate the next row. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    static seq_t *row = NULL;
    static size_t row_len = 0, row_cap = 0;  (void)row_cap;   /* unused, keep for symmetry */

    size_t index = (size_t)n;             /* assume n fits in size_t for cached range */

    /* Initialise state on first call. */
    if (seq == NULL) {
        /* start with axiom row: "1" */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;

        row_cap = 64;
        row = (seq_t *)malloc(row_cap * sizeof(seq_t));
        if (!row) { free(seq); return (seq_t)0; }

        row[0] = (seq_t)1;
        row_len = 1;
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Grow sequence until it contains the requested index. */
    while (seq_len <= index) {
        /* Compute length of next morphic row. */
        size_t new_row_len = 0;
        for (size_t i = 0; i < row_len; ++i) {
            new_row_len += (row[i] == (seq_t)1) ? 3 : 2;
        }

        /* Allocate buffer for the new row. */
        seq_t *new_row = (seq_t *)malloc(new_row_len * sizeof(seq_t));
        if (!new_row) return (seq_t)0;

        /* Expand current row according to the morphism. */
        size_t pos = 0;
        for (size_t i = 0; i < row_len; ++i) {
            if (row[i] == (seq_t)1) {
                new_row[pos++] = (seq_t)1;
                new_row[pos++] = (seq_t)1;
                new_row[pos++] = (seq_t)0;
            } else {
                new_row[pos++] = (seq_t)1;
                new_row[pos++] = (seq_t)0;
            }
        }

        /* Ensure the concatenated sequence has enough capacity. */
        if (seq_cap < seq_len + new_row_len) {
            size_t new_cap = seq_cap * 2;
            if (new_cap < seq_len + new_row_len) new_cap = seq_len + new_row_len;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) { free(new_row); return (seq_t)0; }
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Append the new row into the concatenated sequence. */
        for (size_t i = 0; i < new_row_len; ++i)
            seq[seq_len + i] = new_row[i];
        seq_len += new_row_len;

        /* Replace the old row with the new one. */
        free(row);
        row = new_row;
        row_len = new_row_len;
        row_cap = new_row_len;   /* not used further; kept for symmetry */
    }

    return seq[index];
}
