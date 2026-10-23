/*
 * A186695  A Galton triangle: T(n,k) = (2k-1)*(T(n-1,k) + T(n-1,k-1)): a type
 *          B analog of the ordered Bell numbers A019538.
 * https://oeis.org/A186695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the flattened Galton triangle * T(n,k) =
 *            (2k-1)*(T(n-1,k)+T(n-1,k-1)), T(1,1)=1. * a(1) = 1, a(2) = 1,
 *            a(3) = 3, a(4) = 1,
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static state for memoisation */
static seq_t **triangle = NULL;   /* array of rows (each row is seq_t*)   */
static size_t *row_start = NULL;  /* 1‑based start position of each row  */
static size_t num_rows = 0;       /* number of rows currently stored      */
static size_t cap_rows = 0;       /* capacity of the two arrays            */

/* enlarge capacity */
static void grow_capacity(void) {
    size_t new_cap = (cap_rows == 0) ? 1 : cap_rows * 2;
    seq_t **t_new = (seq_t **)realloc(triangle, new_cap * sizeof(*triangle));
    if (!t_new) abort();
    triangle = t_new;

    size_t *s_new = (size_t *)realloc(row_start, new_cap * sizeof(*row_start));
    if (!s_new) abort();
    row_start = s_new;

    cap_rows = new_cap;
}

/* build the next row (row number = num_rows + 1) and append it */
static void build_next_row(void) {
    size_t row_no = num_rows + 1;          /* 1‑based row number, also its length */
    seq_t *row = (seq_t *)malloc(row_no * sizeof(seq_t));
    if (!row) abort();

    if (row_no == 1) {
        row[0] = (seq_t)1;                 /* T(1,1) = 1 */
    } else {
        seq_t *prev = triangle[num_rows - 1];
        const size_t prev_len = row_no - 1;

        /* first element T(row_no,1) = 1 */
        row[0] = (seq_t)1;

        /* remaining elements */
        for (size_t k = 2; k <= row_no; ++k) {
            seq_t sum = 0;
            /* T(row_no-1,k)  index k-1 in previous row */
            if (k - 1 < prev_len)
                sum += prev[k - 1];
            /* T(row_no-1,k-1)  index k-2 in previous row */
            if (k - 2 < prev_len)
                sum += prev[k - 2];
            row[k - 1] = sum * (seq_t)(2 * k - 1);
        }
    }

    if (num_rows >= cap_rows)
        grow_capacity();

    triangle[num_rows] = row;

    /* compute start position of this row */
    size_t start_pos = (num_rows == 0) ? 1
                                       : row_start[num_rows - 1] + num_rows;
    row_start[num_rows] = start_pos;

    ++num_rows;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0)                  /* offset is 1; undefined for 0 */
        return (seq_t)0;

    /* ensure enough rows were built to cover index n */
    while (num_rows == 0 ||
           row_start[num_rows - 1] + num_rows <= (idx_t)n)
        build_next_row();

    /* locate the row that contains position n */
    size_t r = 0;
    while (r < num_rows &&
           row_start[r] + (r + 1) <= (idx_t)n)
        ++r;

    size_t start_r = row_start[r];              /* 1‑based start of row r+1 */
    size_t col     = (size_t)(n - start_r + 1); /* 1‑based column in that row */

    return triangle[r][col - 1];
}
