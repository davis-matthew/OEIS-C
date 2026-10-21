/*
 * A096587  Triangle read by rows: T(n,k)=number of Catalan knight paths in
 *          Quadrant I from (0,0) to (n,k), for 0 <= k <= 2*n, n >= 0. A
 *          Catalan knight moves (1 right and 2 up) or (1 right and 2 down) or
 *          (2 right and 1 up) or (2 right and 1 down).
 * https://oeis.org/A096587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 256, u32 n <= 732, u64 n <= 2503,
 *            u128 n <= 9028, big any n
 * Helpers:   get_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A096587 – Catalan knight paths in Quadrant I.
 *
 *  a(n) is the flattened triangle read by rows:
 *      a(n) = T(r,c) where r = floor(sqrt(n)), c = n - r^2 .
 *
 *  Triangle recurrence (rows are numbered from 0):
 *      T(0,0) = 1
 *      T(1,2) = 1
 *      For n >= 2:
 *          T(n,0) = T(n-2,1) + T(n-1,2)
 *          T(n,1) = T(n-2,0) + T(n-2,2) + T(n-1,3)
 *          For k >= 2:
 *              T(n,k) = T(n-2,k-1) + T(n-2,k+1)
 *                               + T(n-1,k-2) + T(n-1,k+2)
 *
 *  The function builds the triangle up to the required row on the first
 *  call and caches it for subsequent calls.  All arithmetic is performed
 *  in the user‑provided type seq_t; intermediate values are kept in
 *  seq_t, so arbitrary‑precision builds are exact.
 */

#include <stdlib.h>
#include "../utils/get_val.h"

static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    static seq_t **triangle = NULL;   /* rows of the triangle */
    static idx_t  max_row   = 0;      /* number of rows currently computed */

    /* determine row and column in the triangle */
    idx_t row = isqrt(n);
    while ((row + 1) * (row + 1) <= n) row++;
    while (row * row > n) row--;
    idx_t col = n - row * row;   /* 0 <= col <= 2*row */

    /* expand storage and compute rows as needed */
    if (row >= max_row) {
        /* allocate or grow the array of row pointers */
        idx_t new_rows = row + 1;
        seq_t **new_tri = (seq_t **)malloc(new_rows * sizeof(seq_t *));
        if (!new_tri) return 0;        /* allocation failure */
        for (idx_t i = 0; i < max_row; ++i) new_tri[i] = triangle[i];
        for (idx_t i = max_row; i < new_rows; ++i) new_tri[i] = NULL;
        free(triangle);
        triangle = new_tri;

        /* build new rows */
        for (idx_t r = max_row; r < new_rows; ++r) {
            size_t len = 2 * r + 1;
            seq_t *row_vec = (seq_t *)calloc(len, sizeof(seq_t));
            if (!row_vec) return 0;    /* allocation failure */

            if (r == 0) {
                /* T(0,0) = 1 */
                row_vec[0] = (seq_t)1;
            } else if (r == 1) {
                /* T(1,2) = 1, other entries are 0 */
                if (len > 2) row_vec[2] = (seq_t)1;
            } else {
                for (idx_t k = 0; k < len; ++k) {
                    seq_t val = 0;
                    if (k == 0) {
                        val = get_val((const seq_t **)triangle, r - 2, 1) +
                              get_val((const seq_t **)triangle, r - 1, 2);
                    } else if (k == 1) {
                        val = get_val((const seq_t **)triangle, r - 2, 0) +
                              get_val((const seq_t **)triangle, r - 2, 2) +
                              get_val((const seq_t **)triangle, r - 1, 3);
                    } else {
                        val = get_val((const seq_t **)triangle, r - 2, k - 1) +
                              get_val((const seq_t **)triangle, r - 2, k + 1) +
                              get_val((const seq_t **)triangle, r - 1, k - 2) +
                              get_val((const seq_t **)triangle, r - 1, k + 2);
                    }
                    row_vec[k] = val;
                }
            }
            triangle[r] = row_vec;
        }
        max_row = new_rows;
    }

    return triangle[row][col];
}
