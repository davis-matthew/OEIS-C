/*
 * A193731  Mirror of the triangle A193730.
 * https://oeis.org/A193731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,0)=1; T(1,0)=1; T(1,1)=2; For n ≥ 2: T(n,0)=3*T(n-1,0);
 *            T(n,n)=2*T(n-1,n-1); T(n,k)=2*T(n-1,k-1)+3*T(n-1,k) (1 ≤ k ≤
 *            n-1). a(n) is obtained by reading the rows of T from k=0 to k=n.
 *            That is, let r be the unique integer with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2; let offset = n - r(r+1)/2; then a(n)=T(r...
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 139, u64 n <= 472,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t **triangle = NULL;   /* rows of the triangle T(n,k) */
static idx_t max_row = 0;         /* highest row that has been built */

/* Build rows up to r, inclusive.  The triangle is stored in a static array.
   This function is unsynchronised but is sufficient for the required usage. */
static void build_rows_to(idx_t r)
{
    if (triangle == NULL) {
        /* Allocate space for the first row pointer. */
        triangle = (seq_t **)calloc(1, sizeof(seq_t *));
        max_row = 0;

        /* Row 0: only one element 1. */
        triangle[0] = (seq_t *)calloc(1, sizeof(seq_t));
        triangle[0][0] = (seq_t)1;
    }

    if (r <= max_row)
        return;

    /* Resize the array of row pointers. */
    triangle = (seq_t **)realloc(triangle, (r + 1) * sizeof(seq_t *));

    for (idx_t i = max_row + 1; i <= r; ++i) {
        /* Row i has i+1 elements. */
        triangle[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));

        if (i == 1) {  /* base case for row 1 */
            triangle[i][0] = (seq_t)1;
            triangle[i][1] = (seq_t)2;
            continue;
        }

        /* Left boundary: T(i,0) = 3 * T(i-1,0). */
        triangle[i][0] = (seq_t)3 * triangle[i - 1][0];

        /* Interior elements: T(i,k) = 2*T(i-1,k-1) + 3*T(i-1,k). */
        for (idx_t k = 1; k < i; ++k)
            triangle[i][k] = (seq_t)2 * triangle[i - 1][k - 1]
                           + (seq_t)3 * triangle[i - 1][k];

        /* Right boundary: T(i,i) = 2 * T(i-1,i-1). */
        triangle[i][i] = (seq_t)2 * triangle[i - 1][i - 1];
    }

    max_row = r;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Ensure all rows up to r are built. */
    build_rows_to(r);

    /* Offset within the row. */
    idx_t offset = n - r * (r + 1) / 2;   /* 0 ≤ offset ≤ r */

    return triangle[r][offset];
}
