/*
 * A058692  a(n) = B(n) - 1, where B(n) = Bell numbers, A000110.
 * https://oeis.org/A058692
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(n) - 1, where B(n) is the n-th Bell number. Bell
 *            numbers are generated with the Bell triangle: row[0] = 1; for
 *            each new row, first element is last element of previous row,
 *            subsequent elements are last element + element above
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the Bell triangle row and Bell numbers */
static seq_t *row = NULL;        /* current Bell triangle row */
static size_t row_len = 0;       /* length of current row (>=1) */
static seq_t *bell = NULL;      /* array of Bell numbers, bell[0] = B(0) */
static size_t bell_len = 0;      /* number of Bell numbers stored */
static size_t bell_cap = 0;      /* capacity of bell array */

/* Compute a(n) = B(n) - 1 for n >= 2.  For n < 2, return 0. */
seq_t a(idx_t n)
{
    /* a(0) and a(1) are undefined for this sequence; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Initialise tables on first call */
    if (bell_len == 0) {
        bell_cap = 4;
        bell = (seq_t *)malloc(bell_cap * sizeof(seq_t));
        if (!bell) return (seq_t)0;    /* memory error */
        bell[0] = (seq_t)1;            /* B(0) = 1 */
        bell_len = 1;

        row_len = 1;
        row = (seq_t *)malloc(row_len * sizeof(seq_t));
        if (!row) return (seq_t)0;
        row[0] = (seq_t)1;            /* first row of Bell triangle */
    }

    /* Extend tables up to requested n */
    while (bell_len <= n) {
        size_t new_len = bell_len + 1;   /* length of the new row */

        /* Ensure capacity for the Bell number array */
        if (bell_len >= bell_cap) {
            size_t new_cap = bell_cap * 2;
            seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            bell = tmp;
            bell_cap = new_cap;
        }

        /* Allocate new row */
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_row) return (seq_t)0;

        /* First element is last element of previous row */
        new_row[0] = row[row_len - 1];

        /* Build the rest of the row */
        for (size_t j = 1; j < new_len; ++j)
            new_row[j] = new_row[j - 1] + row[j - 1];

        /* The first element of the new row is B(new_len-1) */
        bell[bell_len] = new_row[0];

        /* Free old row, advance */
        free(row);
        row = new_row;
        row_len = new_len;
        ++bell_len;
    }

    /* Return B(n) - 1 */
    return bell[n] - (seq_t)1;
}
