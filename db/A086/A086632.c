/*
 * A086632  Triangle of coefficients, read by rows, where T(n,k) is the
 *          coefficient of x^n*y^k in f(x,y) that satisfies f(x,y) = 1/(1-x) -
 *          x^2/(1-x)^3 + xy*f(x,y)^3.
 * https://oeis.org/A086632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 40, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:
 *  T(n,0) = 1 - C(n,2)          (C(n,2)=n*(n-1)/2 for n ≥ 2)
 *  For n ≥ 1, k ≥ 1:
 *    T(n,k) = Σ_{n1+n2+n3=n-1,  k1+k2+k3=k-1}  T(n1,k1)*T(n2,k2)*T(n3,k3)
 *
 *  a(·) is the flattened triangle read by rows:
 *    a(n) = T(r,c)  where r is the row containing index n
 *    (row starts at index 0, each row r has r+1 values).
 */
static seq_t **rows          = NULL;   /* pointers to rows of T(n,·)          */
static size_t *row_start     = NULL;   /* start index of each row in flattening */
static size_t rows_cap       = 0;       /* capacity of rows[] and row_start[]   */
static size_t rows_cnt       = 0;       /* number of computed rows              */
static size_t cum_terms      = 0;       /* total number of flattened terms       */

static void ensure_rows_upto(idx_t target)
{
    /* Allocate storage if first call */
    if (rows_cap == 0) {
        rows_cap   = 8;
        rows       = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        row_start  = (size_t *)malloc(rows_cap * sizeof(size_t));
    }

    /* Generate rows until we cover index `target` */
    while (cum_terms <= (size_t)target) {
        if (rows_cnt >= rows_cap) {
            size_t new_cap = rows_cap << 1;
            rows       = (seq_t **)realloc(rows,       new_cap * sizeof(seq_t *));
            row_start  = (size_t *)realloc(row_start, new_cap * sizeof(size_t));
            rows_cap = new_cap;
        }

        idx_t n   = rows_cnt;        /* new row number                     */
        size_t start = cum_terms;    /* start index of this row in flatten  */
        row_start[n] = start;

        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        /* Coefficient T(n,0) */
        if (n < 2) {
            row[0] = 1;
        } else {
            seq_t bin2 = (seq_t)n * ((seq_t)n - 1) / 2;
            row[0] = 1 - bin2;
        }

        /* Coefficients T(n,k) for k ≥ 1 */
        if (n > 0) {
            for (idx_t k = 1; k <= n; ++k) {
                seq_t sum = 0;
                for (idx_t n1 = 0; n1 <= n - 1; ++n1) {
                    for (idx_t n2 = 0; n2 <= n - 1 - n1; ++n2) {
                        idx_t n3 = n - 1 - n1 - n2;
                        for (idx_t k1 = 0; k1 <= n1; ++k1) {
                            for (idx_t k2 = 0; k2 <= n2; ++k2) {
                                idx_t k3 = (k - 1) - k1 - k2;
                                if (k3 > n3) continue;
                                seq_t prod = rows[n1][k1];
                                prod = prod * rows[n2][k2];
                                prod = prod * rows[n3][k3];
                                sum += prod;
                            }
                        }
                    }
                }
                row[k] = sum;
            }
        }

        rows[rows_cnt] = row;
        cum_terms += (n + 1);
        rows_cnt++;
    }
}

seq_t a(idx_t n)
{
    ensure_rows_upto(n);

    /* Find the row containing the flattened index `n` */
    size_t row = 0;
    while (row + 1 < rows_cnt && row_start[row + 1] <= (size_t)n)
        ++row;

    size_t offset = (size_t)n - row_start[row];
    return rows[row][offset];
}
