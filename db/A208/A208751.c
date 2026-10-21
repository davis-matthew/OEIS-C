/*
 * A208751  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208752; see the Formula section.
 * https://oeis.org/A208751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^{k} in u(m,x), where m = smallest
 *            integer with m(m+1)/2 >= n, and k = n - (m-1)m/2 - 1. The
 *            recurrence for T(m,k) is T(m,k) = 2*T(m-1,k) + T(m-1,k-1) -
 *            T(m-2,k) + T(m-2,k-1) with base values T(1,0)=1, T(2,0)=1,
 *            T(2,1)=2, and T(m,k)=0 if k<0 or k>=m
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>

/* Internal storage for triangle rows.  rows[n] holds T(n,k) for 0<=k<n.  */
static seq_t **rows = NULL;      /* rows[1..] (rows[0] unused) */
static size_t  rows_cap = 0;     /* capacity of rows pointer array */
static size_t  max_rows = 0;     /* number of rows currently computed */

/* Ensure rows[1..r_needed] are computed. */
static void ensure_rows_to(size_t r_needed)
{
    if (max_rows > r_needed)   /* already sufficient */
        return;

    /* Grow the rows pointer array if necessary. */
    if (rows_cap <= r_needed) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 4;
        if (new_cap <= r_needed)
            new_cap = r_needed + 1;   /* need indices up to r_needed */
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp)
            abort();
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Build rows incrementally. */
    while (max_rows <= r_needed) {
        size_t cur = max_rows + 1;       /* next row index (1‑based) */
        seq_t *row = (seq_t *)malloc(cur * sizeof(seq_t));
        if (!row)
            abort();

        if (cur == 1) {          /* T(1,0) = 1 */
            row[0] = (seq_t)1;
        } else if (cur == 2) {   /* T(2,0)=1, T(2,1)=2 */
            row[0] = (seq_t)1;
            row[1] = (seq_t)2;
        } else {                 /* recurrence for cur >= 3 */
            seq_t *prev1 = rows[cur - 1];   /* T(cur-1,*)  */
            seq_t *prev2 = rows[cur - 2];   /* T(cur-2,*)  */
            for (size_t k = 0; k < cur; ++k) {
                seq_t t1 = (k < cur - 1) ? prev1[k] : 0;                 /* T(cur-1,k) */
                seq_t t2 = (k > 0)                  ? prev1[k - 1] : 0;     /* T(cur-1,k-1) */
                seq_t t3 = (k < cur - 2) ? prev2[k] : 0;                 /* T(cur-2,k) */
                seq_t t4 = (k > 0 && k - 1 < cur - 2) ? prev2[k - 1] : 0; /* T(cur-2,k-1) */
                row[k] = (t1 << 1) + t2 - t3 + t4;   /* 2*t1 + t2 - t3 + t4 */
            }
        }

        rows[max_rows + 1] = row;
        ++max_rows;
    }
}

/* Public entry point ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n = 0; never called with 0 */
        return 0;

    /* Find smallest m such that m*(m+1)/2 >= n.  */
    size_t m = 1;
    while (((size_t)m * (m + 1)) / 2 < (size_t)n)
        ++m;

    /* Offset within the m-th row (0‑based). */
    size_t prev_total = ((size_t)(m - 1) * m) / 2;
    size_t offset = (size_t)n - prev_total - 1;   /* 0 <= offset < m */

    /* Ensure required rows are computed. */
    ensure_rows_to(m);

    /* Return the coefficient. */
    return rows[m][offset];
}
