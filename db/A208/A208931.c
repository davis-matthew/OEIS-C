/*
 * A208931  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208932; see the Formula section.
 * https://oeis.org/A208931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for triangle A208931 (coefficients of u(n,x))
 *
 * u(1,x) = 1,   v(1,x) = 1
 * u(n,x) = u(n‑1,x) + 2*x*v(n‑1,x)
 * v(n,x) = (x+1)*u(n‑1,x) + (x+1)*v(n‑1,x) + 1
 *
 * The flattened sequence a(n) is read row‑by‑row from the coefficients of u(n,x). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    idx_t start = (r - 1) * r / 2;      /* first index of this row (1‑based) */
    idx_t pos   = n - start;            /* position within the row (1‑based) */

    /* Static storage for the polynomial coefficients */
    static seq_t **u = NULL;            /* u[row][k]  (0 ≤ k < row)   */
    static seq_t **v = NULL;            /* v[row][k]  (0 ≤ k < row)   */
    static idx_t  max_row_computed = 0; /* highest row that has been built */
    static size_t capacity = 0;         /* allocated number of rows */

    /* Grow the pointer array if needed */
    if ((size_t)r >= capacity) {
        size_t new_cap = (size_t)r + 1;
        u = (seq_t**)realloc(u, new_cap * sizeof(seq_t*));
        v = (seq_t**)realloc(v, new_cap * sizeof(seq_t*));
        capacity = new_cap;
    }

    /* Build rows up to r, if not already built */
    if (max_row_computed < r) {
        /* Base case: row 1 */
        if (max_row_computed == 0) {
            u[1] = (seq_t*)malloc(1 * sizeof(seq_t));
            v[1] = (seq_t*)malloc(1 * sizeof(seq_t));
            u[1][0] = (seq_t)1;
            v[1][0] = (seq_t)1;
            max_row_computed = 1;
        }
        /* Compute subsequent rows */
        for (idx_t cur = max_row_computed + 1; cur <= r; ++cur) {
            u[cur] = (seq_t*)malloc(cur * sizeof(seq_t));
            v[cur] = (seq_t*)malloc(cur * sizeof(seq_t));
            for (idx_t k = 0; k < cur; ++k) {
                /* u(cur,k) = u(cur-1,k) + 2 * v(cur-1,k-1) */
                seq_t val_u = 0;
                if (k < cur - 1)
                    val_u += u[cur-1][k];
                if (k >= 1)
                    val_u += (seq_t)2 * v[cur-1][k-1];
                u[cur][k] = val_u;

                /* v(cur,k) = u(cur-1,k) + u(cur-1,k-1) + v(cur-1,k) + v(cur-1,k-1) + (k==0?1:0) */
                seq_t val_v = 0;
                if (k < cur - 1)
                    val_v += u[cur-1][k];
                if (k >= 1)
                    val_v += u[cur-1][k-1];
                if (k < cur - 1)
                    val_v += v[cur-1][k];
                if (k >= 1)
                    val_v += v[cur-1][k-1];
                if (k == 0)
                    val_v += (seq_t)1;
                v[cur][k] = val_v;
            }
        }
        max_row_computed = r;
    }

    /* Retrieve the requested coefficient */
    return u[r][pos - 1];
}
