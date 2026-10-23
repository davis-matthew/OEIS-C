/*
 * A136674  Triangle T(n,k) read by rows: coefficient [x^k] of the polynomial
 *          p(n,x) with p(0,x) = 1, p(1,x) = 2 - x, p(2,x) = 1 - 4*x + x^2 and
 *          p(n,x) = (2-x)*p(n-1,x) - p(n-2,x) if n>2.
 * https://oeis.org/A136674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, sign
 * Formula:   T(0,0)=1, T(1,0)=2, T(1,1)=-1, T(2,0)=1, T(2,1)=-4, T(2,2)=1.
 *            For n≥3, T(n,k)=2*T(n-1,k)-T(n-2,k)-T(n-1,k-1) (missing terms
 *            treated as 0). Sequence a(n) is obtained by reading the triangle
 *            rows consecutively: a(0)=1, a(1)=2, a(2)=-1, a(3)=1, …
 * Range:     i8 n <= 29, i16 n <= 95, i32 n <= 333, i64 n <= 1195,
 *            i128 n <= 4597, big any n
 * Helpers:   new_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/new_row.h"

/* static cache of rows */
static seq_t **rows = NULL;          /* pointers to rows */
static idx_t rows_used = 0;          /* number of rows already generated */
static size_t rows_cap = 0;          /* capacity of rows array */

/* allocate a new row of given length */

/* ensure rows 0 … r are computed and stored in rows[] */
static void ensure_rows(idx_t r)
{
    /* lazy allocation of the rows pointer array */
    if (rows_cap == 0) {
        rows_cap = 4;
        rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
    }

    while (rows_used <= r) {
        /* allocate the current row */
        seq_t *row = new_row(rows_used + 1);

        /* fill the row according to the recurrence */
        if (rows_used == 0) {               /* row 0 */
            row[0] = (seq_t)1;
        } else if (rows_used == 1) {       /* row 1 */
            row[0] = (seq_t)2;
            row[1] = (seq_t)-1;
        } else if (rows_used == 2) {       /* row 2 */
            row[0] = (seq_t)1;
            row[1] = (seq_t)-4;
            row[2] = (seq_t)1;
        } else {                           /* rows >= 3 */
            seq_t *prev  = rows[rows_used - 1];
            seq_t *prev2 = rows[rows_used - 2];
            idx_t len = rows_used + 1;

            for (idx_t k = 0; k < len; ++k) {
                seq_t val = (seq_t)0;

                /* 2*T(n-1,k) */
                if (k <= rows_used - 1) {
                    val += (seq_t)2 * prev[k];
                }

                /* -T(n-2,k) */
                if (k <= rows_used - 2) {
                    val -= prev2[k];
                }

                /* -T(n-1,k-1) */
                if (k > 0) {
                    val -= prev[k - 1];
                }

                row[k] = val;
            }
        }

        /* enlarge the rows pointer array if needed */
        if (rows_used >= rows_cap) {
            size_t new_cap = rows_cap * 2;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            rows = tmp;
            rows_cap = new_cap;
        }

        rows[rows_used] = row;
        ++rows_used;
    }
}

/* find the row r such that offset_row(r) <= N < offset_row(r+1) */
static idx_t row_of(idx_t N)
{
    /* approximate using the quadratic formula */
    double t = sqrt((double)N * 8.0 + 1.0);
    idx_t r = (idx_t)((t - 1.0) / 2.0);

    /* correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= N) ++r;
    while (r * (r + 1) / 2 > N) --r;

    return r;
}

/* the required OEIS sequence function */
seq_t a(idx_t n)
{
    /* find the row and column of the nth coefficient */
    idx_t r = row_of(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* compute rows up to r if not already done */
    ensure_rows(r);

    return rows[r][k];
}
