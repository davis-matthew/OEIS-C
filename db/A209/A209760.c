/*
 * A209760  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209759; see the Formula section.
 * https://oeis.org/A209760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(1,x) = v(1,x) = 1 u(n,x) = x*u(n-1,x) + (x+1)*v(n-1,x) v(n,x)
 *            = x*u(n-1,x) + 2*x*v(n-1,x) + 1 a(n) is the nth coefficient of
 *            v(n,x) when all coefficients of each row are concatenated
 *            row‑wise
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the above recurrence with memoised rows of v(x). */
#include <stdlib.h>
#include <stddef.h>

static seq_t **u_rows = NULL;   /* u_row[k] – array of coefficients of u(k,x) */
static seq_t **v_rows = NULL;   /* v_row[k] – array of coefficients of v(k,x) */
static size_t max_row = 0;      /* highest row currently computed */

/* Return the nth element of the OEIS sequence A209760. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* just a safety guard */

    size_t nn = (size_t)n;                /* promote to C integer */
    /* Determine the row that contains the nth coefficient. */
    size_t row = 1;
    size_t cumulative = 0;
    while (cumulative + row < nn) {
        cumulative += row;
        ++row;
    }
    size_t offset = nn - cumulative - 1;  /* 0‑based index within the row */

    /* Compute rows up to 'row' if not already done. */
    if (row > max_row) {
        /* Ensure the pointer arrays are large enough. */
        u_rows = (seq_t**)realloc(u_rows, (row + 1) * sizeof(seq_t*));
        v_rows = (seq_t**)realloc(v_rows, (row + 1) * sizeof(seq_t*));

        for (size_t k = max_row + 1; k <= row; ++k) {
            seq_t *u_cur = (seq_t*)malloc(k * sizeof(seq_t));
            seq_t *v_cur = (seq_t*)malloc(k * sizeof(seq_t));

            if (k == 1) {                     /* base case */
                u_cur[0] = 1;
                v_cur[0] = 1;
            } else {
                seq_t *u_prev = u_rows[k - 1];
                seq_t *v_prev = v_rows[k - 1];
                size_t prev_len = k - 1;

                for (size_t j = 0; j < k; ++j) {
                    seq_t uval = 0;
                    if (j > 0) {
                        uval += u_prev[j - 1];
                        if (j < prev_len) uval += v_prev[j];
                        uval += v_prev[j - 1];
                    } else {
                        uval += v_prev[0];
                    }

                    seq_t vval = 0;
                    if (j > 0) {
                        vval += u_prev[j - 1];
                        vval += (seq_t)2 * v_prev[j - 1];
                    }
                    if (j == 0) vval += 1;

                    u_cur[j] = uval;
                    v_cur[j] = vval;
                }
            }

            u_rows[k] = u_cur;
            v_rows[k] = v_cur;
        }
        max_row = row;
    }

    return v_rows[row][offset];
}
