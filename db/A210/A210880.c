/*
 * A210880  Triangular array U(n,k) of coefficients of polynomials defined in
 *          Comments.
 * https://oeis.org/A210880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A210880:  Triangular array U(n,k) of coefficients of  
 * the polynomials u(n,x) defined by
 *   u(1)=1,
 *   u(2,x)=x+2,
 *   u(n,x)=x*u(n-1,x)+(x²)*u(n-2,x)+n*(x+1)   for n>2.
 * u(n,x)=∑_{k=1}^{n} U(n,k)·x^{k-1}.
 * The linearised sequence a(n) is obtained by listing the rows
 * in order: a(1)=U(1,1), a(2)=U(2,1), a(3)=U(2,2),
 * a(4)=U(3,1), etc.  The recurrence for the coefficients is
 *      U(n,1)=n,
 *      U(n,2)=U(n-1,1)+n,
 *      U(n,k)=U(n-1,k-1)+U(n-2,k-2)   for k≥3.
 *
 * This implementation memoises rows 1..r that are needed for a(n);
 * it works for any integer type seq_t (fixed‑size or arbitrary‑precision)
 * and any unsigned idx_t as index type.  No external libraries beyond
 * the standard C headers are used.
 */

#include <stdlib.h>
#include <math.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≥ n. */
    idx_t r = (idx_t)((sqrt((double)1 + 8.0 * (double)n) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n) ++r;

    /* Column within the row (1‑based). */
    idx_t offset = r * (r - 1) / 2;
    idx_t c = n - offset;  /* 1‑based column */

    /* Memoised rows.  rows[i] points to row i+1. */
    static seq_t **rows = NULL;
    static size_t rows_cap = 0;  /* number of rows currently stored */

    if (rows_cap < r) {
        /* Extend the array of row pointers */
        seq_t **tmp = (seq_t**)realloc(rows, r * sizeof(seq_t*));
        if (!tmp && r > 0) return (seq_t)0;   /* allocation failure */
        rows = tmp;

        size_t old_cap = rows_cap;
        rows_cap = r;

        for (size_t m = old_cap + 1; m <= rows_cap; ++m) {
            size_t sz = m;                      /* number of coefficients */
            seq_t *row = (seq_t*)malloc(sz * sizeof(seq_t));
            if (!row) return (seq_t)0;          /* allocation failure */

            if (m == 1) {
                row[0] = (seq_t)1;
            } else if (m == 2) {
                row[0] = (seq_t)2;
                row[1] = (seq_t)1;
            } else {
                /* U(m,1) = m */
                row[0] = (seq_t)m;
                /* U(m,2) = U(m-1,1)+m */
                row[1] = rows[m - 2][0] + (seq_t)m;
                /* For k≥3 : U(m,k)=U(m-1,k-1)+U(m-2,k-2) */
                for (idx_t k = 3; k <= (idx_t)sz; ++k) {
                    seq_t val1 = rows[m - 2][k - 2];   /* U(m-1,k-1) */
                    seq_t val2 = rows[m - 3][k - 3];   /* U(m-2,k-2) */
                    row[k - 1] = val1 + val2;
                }
            }
            rows[m - 1] = row;
        }
    }

    return rows[r - 1][c - 1];
}
