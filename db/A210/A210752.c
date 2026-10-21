/*
 * A210752  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210751; see the Formula section.
 * https://oeis.org/A210752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained from the coefficients of the polynomial v(n,x)
 *            defined by u(1,x)=1, v(1,x)=1, u(n,x) = (x+1)*u(n-1,x) +
 *            x*v(n-1,x) + 1, v(n,x) = (x+1)*u(n-1,x) + 2x*v(n-1,x) + 1. The
 *            sequence a(n) is the concatenation of the rows of coefficients
 *            of v(n,x), read row by row
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) for any n ≥ 1.  It stores the
   polynomials for the largest row computed so far and reuses them
   for subsequent calls. */

#include <stdlib.h>

static idx_t max_row = 0;              /* largest row computed so far   */
static seq_t *u_buf = NULL;            /* coefficients of u(n,x) for row max_row */
static seq_t *v_buf = NULL;            /* coefficients of v(n,x) for row max_row */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset 1, a(0) undefined but not used */
    /* find the row r that contains the n‑th term */
    idx_t row = 1;
    idx_t cum = 1;                /* sum of lengths up to current row */
    while (cum < n) {
        ++row;
        cum += row;
    }

    /* grow the stored polynomials to the required row */
    if (row > max_row) {
        /* if this is the first call, initialise row 1 */
        if (max_row == 0) {
            u_buf = (seq_t *)malloc(sizeof(seq_t));
            v_buf = (seq_t *)malloc(sizeof(seq_t));
            u_buf[0] = 1;
            v_buf[0] = 1;
            max_row = 1;
        }
        /* iterate from max_row+1 up to the desired row */
        for (idx_t s = max_row + 1; s <= row; ++s) {
            idx_t new_sz = s;
            seq_t *u_new = (seq_t *)malloc(sizeof(seq_t) * new_sz);
            seq_t *v_new = (seq_t *)malloc(sizeof(seq_t) * new_sz);

            idx_t prev_sz = s - 1;
            for (idx_t i = 0; i < new_sz; ++i) {
                seq_t a_coef = (i < prev_sz) ? u_buf[i] : 0;
                seq_t b_coef = (i > 0) ? u_buf[i-1] : 0;
                seq_t c_coef = (i > 0) ? v_buf[i-1] : 0;

                seq_t ua = a_coef + b_coef + c_coef;
                seq_t va = a_coef + b_coef + (c_coef << 1);

                if (i == 0) {
                    ua += 1;
                    va += 1;
                }

                u_new[i] = ua;
                v_new[i] = va;
            }
            free(u_buf);
            free(v_buf);
            u_buf = u_new;
            v_buf = v_new;
        }
        max_row = row;
    }

    /* locate the coefficient inside the row */
    idx_t before = (row - 1) * row / 2;      /* terms before this row */
    idx_t k = n - before - 1;                /* zero‑based index within row */

    return v_buf[k];
}
