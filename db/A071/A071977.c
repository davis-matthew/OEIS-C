/*
 * A071977  Triangle in which first row is {1}; to get n-th row take first n
 *          numbers greater than last number in previous row which are
 *          relatively prime to n.
 * https://oeis.org/A071977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is given by a triangle. Row n (n ≥ 1) contains the
 *            first n integers greater than the last element of row n‑1 that
 *            are coprime with n. The sequence is the concatenation of the
 *            rows, so a(1)=1, a(2)=3, a(3)=5, …
 * Range:     u8 n <= 135, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute greatest common divisor of two seq_t values.  */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the n‑th term of OEIS A071977 (offset 1).  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* not used, but keeps the function total */
        return (seq_t)0;

    /* Find the row that contains the n-th element.           */
    idx_t row = (idx_t)1;
    idx_t cumulative = (idx_t)1;          /* number of elements up to this row   */
    while (cumulative < n) {
        ++row;
        cumulative += row;
    }

    /* Cache of previously generated rows.  */
    static seq_t **rows = NULL;   /* rows[0] is row 1 */
    static size_t rows_count = 0; /* number of cached rows */
    static size_t rows_cap   = 0; /* allocated capacity    */

    /* Expand rows array if necessary.                            */
    if (rows_count < (size_t)row) {
        if (rows_cap < (size_t)row) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 8;
            if (new_cap < (size_t)row)
                new_cap = (size_t)row;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp)
                abort();
            rows = tmp;
            rows_cap = new_cap;
        }

        /* Generate missing rows.                                 */
        while (rows_count < (size_t)row) {
            idx_t r = (idx_t)(rows_count + 1);  /* row number (1‑based) */
            seq_t *row_arr = (seq_t *)malloc(r * sizeof(seq_t));
            if (!row_arr)
                abort();

            if (r == (idx_t)1) {
                /* First row is {1}. */
                row_arr[0] = (seq_t)1;
            } else {
                /* Start searching just above the last element of previous row. */
                seq_t last_prev = rows[rows_count - 1][rows_count - 1];
                seq_t candidate = last_prev + (seq_t)1;
                idx_t pos = 0;
                seq_t r_seq = (seq_t)r;
                while (pos < r) {
                    if (gcd_seq_t(candidate, r_seq) == (seq_t)1)
                        row_arr[pos++] = candidate;
                    ++candidate;
                }
            }
            rows[rows_count++] = row_arr;
        }
    }

    /* Locate the element within its row.                          */
    idx_t prev_count = (row * (row - (idx_t)1)) / (idx_t)2; /* elements before this row */
    idx_t col = n - prev_count;                           /* 1‑based column      */

    return rows[row - (idx_t)1][col - (idx_t)1];
}
