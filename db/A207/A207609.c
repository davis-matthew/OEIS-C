/*
 * A207609  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A207608; see Formula section.
 * https://oeis.org/A207609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207609
 *  Triangle of coefficients T(n,k) of the polynomials v(n,x) defined by
 *
 *     u(n,x) = u(n-1,x) + v(n-1,x)
 *     v(n,x) = 2 x u(n-1,x) + (x+1) v(n-1,x)
 *
 *  with initial conditions u(1,x)=v(1,x)=1.  The coefficients satisfy the
 *  recurrence
 *
 *     T(n,k) = 2 T(n-1,k) + T(n-1,k-1) + T(n-2,k-1) - T(n-2,k)   (n>2)
 *
 *  and the boundary values
 *
 *     T(1,0) = 1,   T(2,0)=1,   T(2,1)=3.
 *
 *  a(n) is the n‑th element in row‑major order of this triangle
 *  (row 1: 1, row 2: 1 3, row 3: 1 8 3, …).
 *
 *  The function below builds the triangle on demand and caches all
 *  previously computed rows so that subsequent calls are O(1).
 *
 *  seq_t and idx_t are supplied by the build system; no arithmetic
 *  exception handling is required.  The implementation works for
 *  arbitrary‑precision seq_t as well as for native integer types.
 *
 *  Complexity: each entry of the triangle is computed once.  For all
 *  n up to 10 000 the total number of entries is <10 000 × 10 001/2 ≈ 5×10⁶,
 *  which is trivial in time and memory.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of rows: rows[0] is row 1, rows[1] is row 2, … */
    static seq_t **rows = NULL;          /* array of pointers to rows */
    static idx_t rows_count = 0;         /* number of rows currently cached */

    /* determine which (row, col) the index n refers to */
    idx_t r = 1;
    idx_t cumsum = 1;                     /* cumulative number of entries up to row r */
    while (cumsum < n) {
        ++r;
        cumsum += r;
    }
    /* at this point r satisfies r*(r+1)/2 >= n */
    idx_t prev_cumsum = r * (r - 1) / 2;  /* entries before row r */
    idx_t col = n - prev_cumsum - 1;      /* 0‑based column index */

    /* generate missing rows up to row r */
    while (rows_count < r) {
        idx_t new_len = rows_count + 1;   /* length of the new row */
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_row) {
            /* allocation failed – terminate the program as this is a fatal error */
            abort();
        }

        if (new_len == 1) {              /* row 1 */
            new_row[0] = 1;
        } else if (new_len == 2) {       /* row 2 */
            new_row[0] = 1;
            new_row[1] = 3;
        } else {                         /* recurrence for n > 2 */
            seq_t *prev  = rows[rows_count - 1];   /* row n-1 */
            seq_t *prev2 = rows[rows_count - 2];   /* row n-2 */
            idx_t len_prev  = new_len - 1;         /* = rows_count */
            idx_t len_prev2 = new_len - 2;         /* = rows_count - 1 */

            for (idx_t k = 0; k < new_len; ++k) {
                seq_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

                if (k < len_prev)                 /* 2*T(n-1,k) */
                    t1 = (seq_t)2 * prev[k];
                if (k > 0)                         /* T(n-1,k-1) */
                    t2 = prev[k - 1];
                if (k > 0 && (k - 1) < len_prev2) /* T(n-2,k-1) */
                    t3 = prev2[k - 1];
                if (k < len_prev2)                 /* T(n-2,k) */
                    t4 = prev2[k];

                new_row[k] = t1 + t2 + t3 - t4;
            }
        }

        /* append the new row to the cache */
        seq_t **rows_new = (seq_t **)
            realloc(rows, (rows_count + 1) * sizeof(seq_t *));
        if (!rows_new) {
            free(new_row);
            abort();
        }
        rows = rows_new;
        rows[rows_count] = new_row;
        ++rows_count;
    }

    return rows[r - 1][col];
}
