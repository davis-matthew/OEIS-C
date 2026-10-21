/*
 * A208911  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208912; see the Formula section.
 * https://oeis.org/A208911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened triangle of
 *            coefficients of the polynomials u(n,x) defined by
 *            u(n,x)=u(n‑1,x)+2x*v(n‑1,x), v(n,x)=x*u(n‑1,x)+(x+1)*v(n‑1,x)+1,
 *            with u(1,x)=1, v(1,x)=1. The triangle r has r coefficients; a(n)
 *            is the coefficient with index k=0…r‑1 in row r where (r‑1)r/2
 *            <...
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build the triangle lazily; store all rows computed so far. */
#include <stdlib.h>

static idx_t max_row = 0;            /* largest row already computed   */
static seq_t **u_rows = NULL;       /* u coefficients, one array per row */
static seq_t **v_rows = NULL;       /* v coefficients, one array per row */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* sequence is defined only for n≥1 */

    /* Find row r containing position n */
    idx_t r = 1;
    idx_t cumulative = 1;           /* terms up to current row r */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    /* cumulative = r(r+1)/2, cumulative - r = (r-1)r/2 */
    idx_t prev_cum = cumulative - r;        /* terms before row r   */
    idx_t k = n - prev_cum - 1;              /* 0‑based index in row r */

    /* Compute all rows up to r if necessary */
    for (idx_t i = max_row + 1; i <= r; ++i) {
        /* Allocate arrays for new row */
        seq_t *u_new = (seq_t *)malloc(sizeof(seq_t) * i);
        seq_t *v_new = (seq_t *)malloc(sizeof(seq_t) * i);
        if (!u_new || !v_new) {          /* allocation failure – abort */
            free(u_new); free(v_new);
            return 0;
        }

        if (i == 1) {                    /* base case: row 1 */
            u_new[0] = (seq_t)1;
            v_new[0] = (seq_t)1;
        } else {
            /* previous row has length i-1 */
            idx_t prev_len = i - 1;
            seq_t *u_prev = u_rows[i - 2];
            seq_t *v_prev = v_rows[i - 2];

            /* compute u(i, x) coefficients */
            u_new[0] = u_prev[0];
            for (idx_t j = 1; j < i; ++j) {
                seq_t a_val = 0;
                if (j < prev_len) a_val = u_prev[j];
                seq_t b_val = v_prev[j - 1];          /* j-1 < prev_len always true */
                u_new[j] = a_val + (seq_t)2 * b_val;
            }

            /* compute v(i, x) coefficients */
            v_new[0] = v_prev[0] + (seq_t)1;
            for (idx_t j = 1; j < i; ++j) {
                seq_t a_val = u_prev[j - 1];          /* j-1 < prev_len always true */
                seq_t b_val = 0;
                if (j < prev_len) b_val = v_prev[j];
                seq_t c_val = v_prev[j - 1];
                v_new[j] = a_val + b_val + c_val;
            }
        }

        /* Store the new row */
        /* Resize the row pointer arrays as needed */
        if (i > max_row) {
            u_rows = (seq_t **)realloc(u_rows, sizeof(seq_t *) * i);
            v_rows = (seq_t **)realloc(v_rows, sizeof(seq_t *) * i);
        }
        u_rows[i - 1] = u_new;
        v_rows[i - 1] = v_new;
    }
    max_row = r > max_row ? r : max_row;

    return u_rows[r - 1][k];
}
