/*
 * A210556  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210555; see the Formula section.
 * https://oeis.org/A210556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) coefficient of the polynomials v(k,x) defined
 *            by the recurrences u(1,x)=v(1,x)=1
 *            u(n,x)=x*u(n-1,x)+x*v(n-1,x)+1 v(n,x)=2x*u(n-1,x)+v(n-1,x)+1 The
 *            sequence is obtained by listing the constant term first and the
 *            higher powers afterward, row by row
 * Range:     u8 n <= 40, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* external integer types supplied by the build environment */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    if (n < 1) return 0;

    /* Find the row r that contains the n-th coefficient. */
    idx_t r   = 1;
    idx_t tot = 1;          /* total coefficients up to current row */
    while (tot < n) {
        ++r;
        tot += r;
    }
    idx_t prev_tot = tot - r;               /* coefficients before this row */
    idx_t idx_row  = n - prev_tot - 1;       /* 0‑based index within the row */
    size_t len = (size_t)r;                 /* row length */

    /* Allocate 4 temporary rows of length 'len' */
    seq_t *u_prev = (seq_t *)malloc(sizeof(seq_t) * len);
    seq_t *v_prev = (seq_t *)malloc(sizeof(seq_t) * len);
    seq_t *u_cur  = (seq_t *)malloc(sizeof(seq_t) * len);
    seq_t *v_cur  = (seq_t *)malloc(sizeof(seq_t) * len);
    if (!u_prev || !v_prev || !u_cur || !v_cur) {
        /* allocation failure – return 0 as a safe fallback */
        free(u_prev); free(v_prev); free(u_cur); free(v_cur);
        return 0;
    }

    /* Row 1 initialization */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Generate subsequent rows up to row r */
    for (idx_t cur = 2; cur <= r; ++cur) {
        size_t cur_len = (size_t)cur;
        /* Constant term (k = 0) */
        u_cur[0] = (seq_t)1;
        v_cur[0] = v_prev[0] + (seq_t)1;

        /* Terms for k ≥ 1 */
        for (size_t k = 1; k < cur_len; ++k) {
            /* u(n,x) coefficient of x^k */
            u_cur[k] = u_prev[k-1] + v_prev[k-1];

            /* v(n,x) coefficient of x^k */
            seq_t term = (seq_t)2 * u_prev[k-1];
            if (k < cur_len-1) {
                term += v_prev[k];
            }
            v_cur[k] = term;
        }

        /* Swap the current and previous rows for the next iteration */
        seq_t *tmp = u_prev; u_prev = u_cur; u_cur = tmp;
        tmp = v_prev; v_prev = v_cur; v_cur = tmp;
    }

    /* The desired coefficient is now in v_prev[idx_row] */
    seq_t result = v_prev[(size_t)idx_row];

    /* Clean up the allocated memory */
    free(u_prev);
    free(v_prev);
    free(u_cur);
    free(v_cur);

    return result;
}
