/*
 * A059432  Triangle formed when cumulative boustrophedon transform is applied
 *          to 1, 0, 0, 0, ..., read by rows in natural order.
 * https://oeis.org/A059432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059432 – Triangle formed by cumulative boustrophedon transform
 *  of the sequence 1,0,0,0,...
 *
 *  Recurrence for the triangle T(i,j):
 *      T(0,0) = 1
 *      T(i,0) = 0   for i > 0
 *      T(i,j) = T(i,j-1) + Σ_{k=i-j}^{i-1} T(i-1,k)
 *                       (1 ≤ j ≤ i)
 *
 *  The sequence a(n) is read row‑major:
 *      a(n) = T(r,c)   where 0 ≤ c ≤ r and
 *      n = r(r+1)/2 + c   (row r has r+1 elements).
 *
 *  The function caches rows up to the largest required row so that
 *  repeated calls are fast.  All casts are explicit to satisfy both
 *  C11 and C++17.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* determine row r and column c for the index n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t c = n - (r * (r + 1)) / 2;

    /* static cache of rows up to the maximum needed */
    static seq_t **triangle = NULL;          /* triangle[r][c]   */
    static idx_t rows_allocated = 0;        /* number of rows allocated */

    if (rows_allocated <= r) {
        /* grow the outer pointer array */
        triangle = (seq_t **)realloc(triangle,
                                    (r + 1) * sizeof *triangle);
        if (!triangle) return 0;  /* out of memory – return 0 */

        /* allocate and compute any new rows */
        for (idx_t i = rows_allocated; i <= r; ++i) {
            triangle[i] = (seq_t *)malloc((i + 1) * sizeof *triangle[i]);
            if (!triangle[i]) return 0;  /* out of memory – return 0 */

            /* first column */
            triangle[i][0] = (i == 0) ? 1 : 0;
            if (i == 0) continue;  /* row 0 finished */

            /* compute the rest of the row */
            seq_t suffix_sum = 0;
            for (idx_t j = 1; j <= i; ++j) {
                /* add T(i-1, i-j) to the suffix sum */
                suffix_sum += triangle[i - 1][i - j];
                triangle[i][j] = triangle[i][j - 1] + suffix_sum;
            }
        }
        rows_allocated = r + 1;
    }

    return triangle[r][c];
}
