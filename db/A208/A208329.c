/*
 * A208329  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208328; see the Formula section.
 * https://oeis.org/A208329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208329: triangle coefficients of recursively generated polynomials.
   The triangle T(n,k) is defined by
        T(0,0) = 1,
        T(1,0) = 0,   T(1,1) = 3,
        for n ≥ 2 and 0 ≤ k ≤ n:
          T(n,k) = T(n-1,k) + T(n-1,k-1) – T(n-2,k-1) + 2*T(n-2,k-2),
          treating missing terms as 0.
   The sequence a(n) lists the triangle row‑by‑row:
        a(1)=T(0,0), a(2)=T(1,0), a(3)=T(1,1), a(4)=T(2,0), …   */

/* Standard includes required for allocation and exit */
#include <stdlib.h>
#include <stdint.h>

/* Static state holding the already computed rows of the triangle */
static seq_t **triangle = NULL;          /* array of pointers to rows */
static idx_t  tri_rows = 0;              /* number of rows computed so far */
static idx_t  max_rows = 0;              /* capacity of the triangle array */

/* Ensure that rows 0 … r are computed and stored in `triangle`. */
static void ensure_rows(idx_t r)
{
    /* Allocate the pointer array on first use. */
    if (triangle == NULL) {
        max_rows = 64;                     /* initial capacity */
        triangle = (seq_t**)malloc(max_rows * sizeof(seq_t*));
        if (!triangle) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < max_rows; ++i) triangle[i] = NULL;
    }

    /* Generate rows until we have row r. */
    while (tri_rows <= r) {
        /* Grow the pointer array if necessary. */
        if (tri_rows >= max_rows) {
            max_rows <<= 1;                /* double capacity */
            seq_t **tmp = (seq_t**)realloc(triangle, max_rows * sizeof(seq_t*));
            if (!tmp) exit(EXIT_FAILURE);
            triangle = tmp;
            for (idx_t i = tri_rows; i < max_rows; ++i) triangle[i] = NULL;
        }

        /* Allocate the new row. */
        size_t len = (size_t)(tri_rows) + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        if (tri_rows == 0) {               /* T(0,0) = 1 */
            row[0] = 1;
        }
        else if (tri_rows == 1) {          /* T(1,0) = 0, T(1,1) = 3 */
            row[0] = 0;
            row[1] = 3;
        }
        else {                             /* recurrence for n ≥ 2 */
            for (size_t k = 0; k < len; ++k) {
                seq_t a = (k <= tri_rows - 1) ? triangle[tri_rows - 1][k] : 0;
                seq_t b = (k >= 1)           ? triangle[tri_rows - 1][k - 1] : 0;
                seq_t c = (k >= 1 && k - 1 <= tri_rows - 2)
                            ? triangle[tri_rows - 2][k - 1] : 0;
                seq_t d = (k >= 2)           ? triangle[tri_rows - 2][k - 2] : 0;
                row[k] = a + b - c + (seq_t)(2) * d;
            }
        }

        triangle[tri_rows] = row;
        ++tri_rows;
    }
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – not used */

    /* Find the row r such that (r+1)(r+2)/2 ≥ n. */
    idx_t r = 0;
    while (1) {
        unsigned __int128 v = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (v >= n) break;
        ++r;
    }

    /* Prefix sum of elements in previous rows. */
    unsigned __int128 prefix = (unsigned __int128)r * (r + 1) / 2;
    idx_t col = (idx_t)(n - prefix - 1);

    ensure_rows(r);
    return triangle[r][col];
}
