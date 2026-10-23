/*
 * A171998  In general, let A(n,k,m) denote the (n,k)-th entry of the inverse
 *          of the matrix consisting of the (n,k)-th m-restrained Stirling
 *          numbers of the second kind (-1)^(n-k) times the number of
 *          permutations of an n-set with k disjoint cycles of length less
 *          than or equal to m, as the (n+1,k+1)-th entry....
 * https://oeis.org/A171998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more, tabl
 * Formula:   a(n) is the n-th term of the lower triangular matrix A(n,k,3)
 *            (m‑restrained Stirling numbers of the second kind, m=3) read
 *            row‑wise. Recurrence used to generate the matrix: A(0,0)=1,
 *            A(n,k)=0 for other k<0 or k>n
 *            A(n,k,3)=A(n-1,k-1,3)+k*A(n-1,k,3)-k(k+1)(k+2)*A(n,k+3,3)
 * Range:     i8 n <= 15, i16 n <= 28, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t **rows = NULL;            /* array of pointers to each row          */
static idx_t   rows_used = 0;          /* number of rows currently allocated    */
static idx_t   rows_cap  = 0;          /* capacity of the rows pointer array    */

/* ensure that rows[0..r] are computed and stored */
static void ensure_rows(idx_t r)
{
    /* initialise rows array if required */
    if (rows == NULL) {
        rows_cap = 1;
        rows = (seq_t **)calloc(rows_cap, sizeof(seq_t *));
        if (rows == NULL) exit(1);      /* out of memory */
        /* row 0: A(0,0)=1 */
        rows[0] = (seq_t *)calloc(1, sizeof(seq_t));
        if (rows[0] == NULL) exit(1);
        rows[0][0] = 1;
        rows_used = 1;
    }

    /* grow the pointer array if needed */
    while (rows_used <= r) {
        if (rows_used >= rows_cap) {
            idx_t new_cap = rows_cap * 2;
            if (new_cap == 0) new_cap = 1;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (tmp == NULL) exit(1);
            rows = tmp;
            rows_cap = new_cap;
        }

        /* compute row 'rows_used' (which is n) */
        idx_t n = rows_used;
        seq_t *row_prev = rows[n - 1];
        seq_t *row = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (row == NULL) exit(1);            /* row[0] stays 0 */

        for (idx_t k = n; k >= 1; --k) {
            /* term1 = A(n-1,k-1) */
            seq_t term1 = row_prev[k - 1];

            /* term2 = A(n-1,k) if k <= n-1 else 0 */
            seq_t term2 = 0;
            if (k <= n - 1) term2 = row_prev[k];

            /* term3 = k(k+1)(k+2) * A(n,k+3) if k+3 <= n */
            seq_t term3 = 0;
            if (k + 3 <= n) {
                seq_t prod = (seq_t)k * (k + 1) * (k + 2);
                term3 = prod * row[k + 3];
            }

            row[k] = term1 + (seq_t)k * term2 - term3;
        }

        rows[n] = row;
        rows_used++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* find the row (r) containing the n-th element */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    if (r * (r + 1) / 2 < n) ++r;

    /* column within that row (1‑based) */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t c = n - prev_sum;          /* 1 <= c <= r */

    ensure_rows(r);
    return rows[r][c];
}
