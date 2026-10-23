/*
 * A046089  Triangle read by rows, the Bell transform of (n+2)!/2 without
 *          column 0.
 * https://oeis.org/A046089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the term at index n in triangle T with * T(n,m) = (2*m +
 *            n - 1) * T(n-1,m) + T(n-1,m-1) * T(n,m)=0 for n<m, T(n,0)=0,
 *            T(1,1)=1 * and the index n is the triangular read order, i.e.
 *            a(1)=T(1,1), * a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* find the row r (n ≥ m ≥ 1) such that T_{r-1} < n ≤ T_r,
     * where T_k = k*(k+1)/2 (triangular numbers). */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* column m of row r */
    idx_t m = n - ((r - 1) * r) / 2;

    /* static storage for the triangle, grown as needed */
    static seq_t **tri = NULL;   /* tri[i] points to row i, 1‑based, tri[i][0] unused */
    static idx_t maxrow = 0;

    if (maxrow < r) {
        /* grow row pointer array if necessary */
        if (tri == NULL) {
            tri = (seq_t **)malloc(sizeof(seq_t *) * (r + 1));
            for (idx_t i = 0; i <= r; ++i) tri[i] = NULL;
        } else if (r >= maxrow + 1) {
            tri = (seq_t **)realloc(tri, sizeof(seq_t *) * (r + 1));
        }

        /* compute new rows from maxrow+1 up to r */
        for (idx_t i = maxrow + 1; i <= r; ++i) {
            tri[i] = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
            tri[i][0] = 0;                     /* unused slot */
            if (i == 1) {                      /* base row */
                tri[1][1] = (seq_t)1;
                continue;
            }
            for (idx_t j = 1; j <= i; ++j) {
                seq_t t1 = 0, t2 = 0;
                if (j <= i - 1) {
                    seq_t coeff = (seq_t)(2 * j + i - 1);
                    t1 = coeff * tri[i - 1][j];
                }
                if (j > 1) t2 = tri[i - 1][j - 1];
                tri[i][j] = t1 + t2;
            }
        }
        maxrow = r;
    }

    return tri[r][m];
}
