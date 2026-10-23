/*
 * A159931  Triangle read by rows: a(1,1) = 1. a(m,m) = product of the terms
 *          in row m-1. For n < m, a(m,n) = a(m-1,n) + (product of the terms
 *          in row m-1).
 * https://oeis.org/A159931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 15, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle recurrence:
   a(1,1)=1.
   Let P_{m-1} be the product of the terms in row m-1.
   Then for 1 ≤ n < m:  a(m,n) = a(m-1,n) + P_{m-1}
   and a(m,m) = P_{m-1}.
   The sequence a(n) is read row‑by‑row, starting with n = 1. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *seq = NULL;          /* array of sequence terms */
    static size_t seq_cap = 0;         /* capacity of seq */
    static idx_t  seq_len = 0;         /* number of terms stored */

    static seq_t *prev = NULL;         /* current row */
    static idx_t  prev_len = 0;        /* length of current row */
    static seq_t  Pprev = 0;           /* product of current row */

    /* Initialise on first invocation */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;     /* allocation failed */
        seq_len = 0;
        prev = NULL;
        prev_len = 0;
        Pprev = 0;
    }

    /* Generate rows until we have at least n terms */
    while (seq_len < n) {
        idx_t m;                /* index of the row to generate */
        if (prev_len == 0) {    /* first row */
            m = 1;
        } else {
            m = prev_len + 1;
        }

        if (m == 1) {           /* handle the first row explicitly */
            /* row 1: single element 1 */
            if (seq_len + 1 > seq_cap) {
                seq_cap = (seq_cap + 1) * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)1;

            /* initialise prev array for the next rows */
            prev = (seq_t *)malloc(sizeof(seq_t));
            prev[0] = (seq_t)1;
            prev_len = 1;
            Pprev = (seq_t)1;
        } else {
            /* generate row m */
            idx_t prev_len_local = prev_len;
            idx_t new_len = prev_len_local + 1;

            /* allocate array for the new row */
            seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!new_row) return (seq_t)0;      /* allocation failed */

            /* compute elements of the new row and accumulate product */
            seq_t prod = (seq_t)1;
            for (idx_t i = 0; i < prev_len_local; ++i) {
                seq_t val = prev[i] + Pprev;
                new_row[i] = val;
                prod *= val;
            }
            /* last element is the product of the previous row */
            new_row[new_len - 1] = Pprev;
            prod *= Pprev;                     /* full product of this row */

            /* extend the sequence array if necessary */
            if (seq_len + new_len > seq_cap) {
                seq_cap = (seq_cap + new_len) * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            /* store new row into the sequence */
            for (idx_t i = 0; i < new_len; ++i) {
                seq[seq_len++] = new_row[i];
            }

            /* update state for next iteration */
            free(prev);
            prev = new_row;
            prev_len = new_len;
            Pprev = prod;
        }
    }

    return seq[n - 1];
}
