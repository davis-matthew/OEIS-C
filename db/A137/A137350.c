/*
 * A137350  A triangular Sequence of coefficients of a three deep polynomial
 *          recursion based on a Chebyshev kind and a Padovan recursion:
 *          Chebyshev; p(x,n)=x*p(x,n-1)-p(x,n-2); Padovan:
 *          a(n)=a(n-2)+a(n-3); Q(x, n) = x*Q(x, n - 2) - Q(x, n - 3).
 * https://oeis.org/A137350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   a(n) is the flattened sequence of the coefficient lists of the
 *            polynomials Q(x,r) defined by Q(x,0) = 1 Q(x,1) = -1 + x Q(x,2)
 *            = x Q(x,r) = x*Q(x,r-2) - Q(x,r-3) for r ≥ 3 The flattened list
 *            (row‑by‑row, starting at n = 1) is OEIS A137350
 * Range:     i8 n <= 41, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*---  static data used by the function ----------------------------------*/
static seq_t **rows         = NULL;   /* pointers to coefficient arrays   */
static size_t *len_rows     = NULL;   /* lengths of each row              */
static idx_t  *start_rows   = NULL;   /* starting index of each row in flat   */
static size_t  rows_cap     = 0;      /* current capacity for the rows    */
static idx_t   total_terms  = 0;      /* total number of terms already stored */
static idx_t   max_row_index = 0;     /* zero‑based index of last ready row */
static int     initialized  = 0;      /* flag whether the table has been set up */

/*---  helper to enlarge the stored row arrays -----------------------------*/
static void ensure_rows_capacity(size_t need)
{
    if (need <= rows_cap)
        return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    rows      = (seq_t**)realloc(rows,      new_cap * sizeof(seq_t*));
    len_rows  = (size_t*)realloc(len_rows,  new_cap * sizeof(size_t));
    start_rows= (idx_t*)realloc(start_rows,new_cap * sizeof(idx_t));
    rows_cap  = new_cap;
}

/*---  initialise the first three rows -----------------------------------*/
static void init_rows(void)
{
    ensure_rows_capacity(3);

    /* Row 0 (r = 0): Q(x,0) = 1, coeffs {1} */
    rows[0]      = (seq_t*)malloc(sizeof(seq_t));
    rows[0][0]   = (seq_t)1;
    len_rows[0]  = 1;
    start_rows[0]= 1;
    total_terms  = 1;

    /* Row 1 (r = 1): Q(x,1) = -1 + x, coeffs {-1,1} */
    rows[1]      = (seq_t*)malloc(2*sizeof(seq_t));
    rows[1][0]   = (seq_t)-1;
    rows[1][1]   = (seq_t)1;
    len_rows[1]  = 2;
    start_rows[1]= 2;
    total_terms  = 3;

    /* Row 2 (r = 2): Q(x,2) = x, coeffs {0,1} */
    rows[2]      = (seq_t*)malloc(2*sizeof(seq_t));
    rows[2][0]   = (seq_t)0;
    rows[2][1]   = (seq_t)1;
    len_rows[2]  = 2;
    start_rows[2]= 4;
    total_terms  = 5;

    max_row_index = 2;
    initialized   = 1;
}

/*---  compute the sequence for any n (>=1) --------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized)
        init_rows();

    /* generate rows until we have at least n terms */
    while (total_terms < n) {
        idx_t next_row = max_row_index + 1;                /* zero‑based   */
        size_t len_prev2 = len_rows[next_row - 2];
        size_t len_prev3 = len_rows[next_row - 3];
        size_t len_next = len_prev2 + 1 > len_prev3 ? len_prev2 + 1 : len_prev3;

        seq_t *next = (seq_t*)malloc(len_next * sizeof(seq_t));
        const seq_t *prev2 = rows[next_row - 2];          /* Q(r-2)       */
        const seq_t *prev3 = rows[next_row - 3];          /* Q(r-3)       */

        size_t i;
        for (i = 0; i < len_next; ++i) {
            seq_t val = 0;
            if (i > 0 && (size_t)(i - 1) < len_prev2)
                val += prev2[i - 1];
            if ((size_t)i < len_prev3)
                val -= prev3[i];
            next[i] = val;
        }

        ensure_rows_capacity(max_row_index + 2);
        rows[next_row]        = next;
        len_rows[next_row]    = len_next;
        start_rows[next_row]  = total_terms + (idx_t)1;
        total_terms          += (idx_t)len_next;
        max_row_index        = next_row;
    }

    /* locate the row which contains the requested index n */
    idx_t row = 0;
    for (; row <= max_row_index; ++row) {
        if (n < start_rows[row] + (idx_t)len_rows[row])
            break;
    }

    return rows[row][(size_t)(n - start_rows[row])];
}
