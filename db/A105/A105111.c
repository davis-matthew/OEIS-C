/*
 * A105111  Triangle read by rows, based on the morphism f: 1->2, 2->3,
 *          3->{3,5,4}, 4->5, 5->6, 6->{6,2,1}. First row is 1. If current row
 *          is a,b,c,..., then the next row is a,b,c,...,f(a),f(b),f(c),...
 * https://oeis.org/A105111
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

/* A105111: Triangle read by rows generated from the morphism
 * f : 1→2, 2→3, 3→{3,5,4}, 4→5, 5→6, 6→{6,2,1}.
 * First row is [1].  If the current row is a,b,c,… then
 * the next row is a,b,c,…,f(a),f(b),f(c),…
 * The sequence a(n) is the flattened list of rows, starting at a(0)=1. */

#include <stdlib.h>

static void ensure_cap(size_t needed, seq_t **arr, size_t *cap) {
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n) {
    /* static state ----------------------------------------------------- */
    static seq_t *seq_arr = NULL;   /* flattened sequence values      */
    static size_t seq_cap = 0;      /* allocated capacity of seq_arr  */
    static size_t seq_len = 0;      /* number of elements in seq_arr  */

    static seq_t *prev_row = NULL;  /* current row being processed   */
    static size_t prev_len = 0;     /* length of current row         */

    /* initialise state on first call -------------------------------- */
    if (prev_row == NULL) {
        /* The very first row is simply [1] */
        prev_len = 1;
        prev_row = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev_row[0] = (seq_t)1;

        seq_arr = (seq_t *)malloc(16 * sizeof(seq_t));
        seq_cap = 16;
        seq_len = 0;
    }

    /* Generate rows until we have at least (n+1) elements in seq_arr ---- */
    while (seq_len <= (size_t)n) {
        /* --- Append current row to the flattened sequence --- */
        ensure_cap(seq_len + prev_len, &seq_arr, &seq_cap);
        for (idx_t i = 0; i < (idx_t)prev_len; ++i)
            seq_arr[seq_len++] = prev_row[i];

        /* --- Compute f(prev_row) (the image of the current row) ---- */
        size_t next_len = 0;
        /* worst‑case bound: every symbol could expand to 3 symbols */
        size_t est_next_len = prev_len * 3;
        seq_t *next_row = (seq_t *)malloc(est_next_len * sizeof(seq_t));

        for (idx_t i = 0; i < (idx_t)prev_len; ++i) {
            unsigned int v = (unsigned int)prev_row[i];
            switch (v) {
                case 1: next_row[next_len++] = (seq_t)2; break;
                case 2: next_row[next_len++] = (seq_t)3; break;
                case 3:
                    next_row[next_len++] = (seq_t)3;
                    next_row[next_len++] = (seq_t)5;
                    next_row[next_len++] = (seq_t)4;
                    break;
                case 4: next_row[next_len++] = (seq_t)5; break;
                case 5: next_row[next_len++] = (seq_t)6; break;
                case 6:
                    next_row[next_len++] = (seq_t)6;
                    next_row[next_len++] = (seq_t)2;
                    next_row[next_len++] = (seq_t)1;
                    break;
                default: /* should never happen */ break;
            }
        }

        /* --- Build the new row: concatenate prev_row and next_row --- */
        size_t new_len = prev_len + next_len;
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));

        for (idx_t i = 0; i < (idx_t)prev_len; ++i)
            new_row[i] = prev_row[i];
        for (idx_t i = 0; i < (idx_t)next_len; ++i)
            new_row[prev_len + i] = next_row[i];

        /* --- Clean up temporary structures --- */
        free(prev_row);
        free(next_row);

        /* --- Prepare for next iteration --- */
        prev_row = new_row;
        prev_len = new_len;
    }

    /* seq_arr now contains all needed terms */
    return seq_arr[(size_t)n];
}
