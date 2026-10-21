/*
 * A208520  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208521; see the Formula section.
 * https://oeis.org/A208520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^{k-1} in u(r,x), * where u and v satisfy
 *            * u(1,x)=1, v(1,x)=1 * u(n,x)=u(n-1,x)+x*v(n-1,x) *
 *            v(n,x)=2x*u(n-1,x)+v(n-1,x)+1 * The sequence is read row‑wise
 *            from the triangle of coefficients of u. * Initial values:
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static tables for already computed rows of u and v */
static seq_t **u_tbl = NULL;
static seq_t **v_tbl = NULL;
static idx_t  max_row = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not used, but keeps function total */

    /* Determine the row r that contains the n‑th term */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Grow tables up to row r if necessary */
    if (r > max_row) {
        /* Extend the pointer arrays */
        u_tbl = (seq_t **)realloc(u_tbl, (size_t)(r + 1) * sizeof(seq_t*));
        v_tbl = (seq_t **)realloc(v_tbl, (size_t)(r + 1) * sizeof(seq_t*));

        /* Compute new rows from max_row+1 up to r */
        for (idx_t row = max_row + 1; row <= r; ++row) {
            size_t len = (size_t)row;                    /* number of coefficients in this row */
            u_tbl[row] = (seq_t *)malloc(len * sizeof(seq_t));
            v_tbl[row] = (seq_t *)malloc(len * sizeof(seq_t));

            if (row == 1) {                         /* base row */
                u_tbl[1][0] = (seq_t)1;
                v_tbl[1][0] = (seq_t)1;
                continue;
            }

            idx_t prev = row - 1;
            size_t prev_len = prev;                  /* length of previous row */

            for (size_t d = 0; d < len; ++d) {
                if (d == 0) {                         /* constant term */
                    u_tbl[row][0] = u_tbl[prev][0];
                    v_tbl[row][0] = v_tbl[prev][0] + (seq_t)1;
                } else {
                    /* u(row,d) = u(prev,d) + v(prev,d-1) (if indices exist) */
                    seq_t term_u  = (d < prev_len) ? u_tbl[prev][d] : (seq_t)0;
                    seq_t term_v  = v_tbl[prev][d - 1];
                    u_tbl[row][d] = term_u + term_v;

                    /* v(row,d) = 2*u(prev,d-1) + v(prev,d) (if indices exist) */
                    seq_t tmp     = (seq_t)2 * u_tbl[prev][d - 1];
                    seq_t term_v2 = (d < prev_len) ? v_tbl[prev][d] : (seq_t)0;
                    v_tbl[row][d] = tmp + term_v2;
                }
            }
        }
        max_row = r;
    }

    /* The first index of this row in the triangle (1‑based) */
    idx_t first_in_row = (r - 1) * r / 2;
    idx_t k            = n - first_in_row;          /* 1‑based column index */

    return u_tbl[r][k - 1];
}
