/*
 * A038622  Triangular array that counts rooted polyominoes.
 * https://oeis.org/A038622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice, changed
 * Formula:   a(n) = T(m,k) where T(0,0) = 1, T(m,0) = 2*T(m-1,0) + T(m-1,1),
 *            T(m,k) = T(m-1,k-1) + T(m-1,k) + T(m-1,k+1) (k ≥ 1). The
 *            sequence a(n) is obtained by reading the triangle row by row
 *            (row 0, then row 1, …). The triangle is right‑triangular, so
 *            a(n) = T(m,k) with m the row and k the column satisfyi...
 * Range:     u8 n <= 20, u16 n <= 77, u32 n <= 252, u64 n <= 945,
 *            u128 n <= 3485, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Typedefs defined by the build environment. */
extern seq_t a(idx_t n);          /* forward declaration may not be needed */

/* Static storage for the computed rows of the triangle. */
static seq_t **triangle = NULL;   /* array of pointers, one per row */
static idx_t   triangle_rows = 0; /* number of rows currently stored */

/* Allocate a new row of the triangle.  The new row has length 'len' (row+1). */
static void add_row(idx_t row) {
    idx_t len = row + 1;
    seq_t *row_arr = (seq_t *)malloc(sizeof(seq_t) * len);
    if (!row_arr) abort();        /* allocation failure */

    if (row == 0) {                /* base row */
        row_arr[0] = (seq_t)1;
    } else {
        seq_t *prev = triangle[row - 1];

        /* first element of the row */
        seq_t first = 2 * prev[0];
        if (row - 1 >= 1)
            first += prev[1];
        row_arr[0] = first;

        /* middle elements */
        for (idx_t k = 1; k < row; ++k) {
            seq_t a = prev[k - 1];
            seq_t b = prev[k];
            seq_t c = (k + 1 < row) ? prev[k + 1] : 0;
            row_arr[k] = a + b + c;
        }

        /* last element of the row – always 1 */
        row_arr[row] = (seq_t)1;
    }

    /* grow the triangle pointer to hold the new row */
    seq_t **new_tri = (seq_t **)realloc(triangle,
                                        sizeof(seq_t *) * (triangle_rows + 1));
    if (!new_tri) abort();
    triangle = new_tri;
    triangle[triangle_rows] = row_arr;
    ++triangle_rows;
}

/* Ensure that all rows up to 'up_to' (inclusive) are computed. */
static void ensure_rows(idx_t up_to) {
    while (triangle_rows <= up_to) {
        add_row(triangle_rows);
    }
}

/* Convert linear index n to (row, column) in the triangle and return T(row,column). */
seq_t a(idx_t n) {
    /* locate the smallest row r with r(r+1)/2 > n */
    idx_t r = 0;
    for (;;) {
        idx_t row_len = r + 1;
        idx_t boundary = r * (r + 1) / 2 + row_len; /* first index beyond this row */
        if (n < boundary)
            break;
        ++r;
    }

    /* compute the column k within that row */
    idx_t leading = r * (r + 1) / 2;
    idx_t k = n - leading;

    /* compute rows up to r if not yet done */
    ensure_rows(r);

    return triangle[r][k];
}
