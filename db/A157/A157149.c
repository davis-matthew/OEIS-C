/*
 * A157149  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) + m*k*(n-k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1 and m = 3, read by rows.
 * https://oeis.org/A157149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 57, u64 n <= 141,
 *            u128 n <= 362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157149 – Triangle T(n,k,3) read by rows.
   Recurrence:
     T(n,k) = (3*(n-k)+1)*T(n-1,k-1) + (3*k+1)*T(n-1,k) + 3*k*(n-k)*T(n-2,k-1)
     with T(n,0) = T(n,n) = 1.
   a(n) is the n‑th entry when the triangle is read row by row. */

#include <stdint.h>
#include <stdlib.h>

/* Helper that returns the starting index of row r
   (row 0 starts at index 0).  Uses 128‑bit arithmetic
   to avoid overflow for very large r. */
static idx_t offset_row(idx_t r)
{
    /* r*(r+1)/2 */
    unsigned __int128 prod = (unsigned __int128)r * (r + 1);
    return (idx_t)(prod >> 1);
}

/* Internal storage of the triangle.
   triangle[i] points to an array of length i+1 containing
   T(i,0..i).  The array is grown lazily.  All rows are stored
   forever; no deallocation is performed. */
static seq_t **triangle = NULL;        /* array of row pointers */
static size_t tri_rows = 0;            /* number of rows stored */
static size_t tri_cap  = 0;            /* allocated capacity */

/* Grow the triangle storage to hold at least `need` rows. */
static void ensure_triangle_capacity(size_t need)
{
    if (tri_cap >= need)
        return;
    size_t new_cap = tri_cap ? tri_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq_t **new_tri = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
    if (!new_tri)
        abort();          /* out of memory */
    triangle = new_tri;
    tri_cap = new_cap;
}

/* Compute rows up to `row` (inclusive).  Row 0 and 1 are
   always present.  Subsequent rows are computed using the
   recurrence. */
static void compute_rows_up_to(idx_t row)
{
    if (row < tri_rows)
        return;

    ensure_triangle_capacity((size_t)row + 1);

    /* Row 0 */
    if (tri_rows == 0) {
        triangle[0] = (seq_t *)malloc(1 * sizeof(seq_t));
        triangle[0][0] = (seq_t)1;
        tri_rows = 1;
    }

    /* Row 1 */
    if (tri_rows == 1) {
        triangle[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        triangle[1][0] = (seq_t)1;
        triangle[1][1] = (seq_t)1;
        tri_rows = 2;
    }

    /* Rows 2 and above */
    for (size_t r = tri_rows; r <= (size_t)row; ++r) {
        seq_t *cur = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        cur[0] = (seq_t)1;
        cur[r] = (seq_t)1;

        seq_t M = (seq_t)3;
        seq_t ONE = (seq_t)1;

        for (size_t j = 1; j < r; ++j) {
            seq_t part1 = ((M * (seq_t)(r - j)) + ONE) * triangle[r - 1][j - 1];
            seq_t part2 = ((M * (seq_t)j) + ONE) * triangle[r - 1][j];
            seq_t part3 = M * (seq_t)j * (seq_t)(r - j) *
                          triangle[r - 2][j - 1];
            cur[j] = part1 + part2 + part3;
        }
        triangle[r] = cur;
    }
    tri_rows = (size_t)row + 1;
}

/* Main sequence function as requested. */
seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1). */
    idx_t r = 0;
    while (offset_row(r + 1) <= n)
        ++r;
    idx_t k = n - offset_row(r);

    /* Ensure the triangle has been built up to this row. */
    compute_rows_up_to(r);

    return triangle[r][k];
}
