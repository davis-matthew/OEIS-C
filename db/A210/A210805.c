/*
 * A210805  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210806; see the Formula section.
 * https://oeis.org/A210805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials u(n,x)
 *   u(n,x) = u(n-1,x) + x v(n-1,x)
 *   v(n,x) = (x+2) u(n-1,x) + (x−1) v(n-1,x) − 1
 * with u(1,x) = 1, v(1,x) = 1.
 * The sequence a(n) is the flattened row‑wise list of the u‑coefficient
 * triangle: a(1) = u(1,0), a(2) = u(2,0), a(3) = u(2,1), a(4) = u(3,0), …
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* safeguard, offset = 1 */

    /* Find the row r that contains the n‑th coefficient.  */
    idx_t r = 1;
    while ((r * (r + 1ull) / 2ull) < n) ++r;

    /* r is the row index, rows are 1‑based; row length = r. */
    idx_t maxlen = r;                  /* maximum array size needed */

    /* Allocate two pairs of arrays for rolling calculation.  */
    seq_t *u_a = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *v_a = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *u_b = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *v_b = (seq_t *)malloc(maxlen * sizeof(seq_t));
    if (!u_a || !v_a || !u_b || !v_b) {
        free(u_a); free(v_a); free(u_b); free(v_b);
        return 0;                      /* allocation failure – unlikely */
    }

    /* Base row (n = 1).  */
    u_a[0] = 1;
    v_a[0] = 1;
    idx_t len_cur = 1;                 /* current row length */

    /* Compute rows 2 … r, swapping pointers each iteration. */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t len_next = row;

        for (idx_t k = 0; k < len_next; ++k) {
            seq_t u_prev   = (k < len_cur) ? u_a[k] : 0;
            seq_t v_prev   = (k < len_cur) ? v_a[k] : 0;
            seq_t v_prev_m = (k > 0)      ? v_a[k - 1] : 0;
            seq_t u_prev_m = (k > 0)      ? u_a[k - 1] : 0;

            u_b[k] = u_prev + v_prev_m;

            seq_t term1 = (k > 0)      ? u_prev_m : 0;
            seq_t term2 = 2 * u_prev;
            seq_t term3 = (k > 0)      ? v_prev_m : 0;
            seq_t term4 = v_prev;
            seq_t sub1  = (k == 0) ? 1 : 0;

            v_b[k] = term1 + term2 + term3 - term4 - sub1;
        }

        /* Swap the pointers for next iteration. */
        seq_t *tmp = u_a; u_a = u_b; u_b = tmp;
        tmp = v_a;        v_a = v_b; v_b = tmp;
        len_cur = len_next;
    }

    /* Extract the requested coefficient from the final row. */
    idx_t prev_total = (r - 1) * r / 2ull;          /* elements before row r */
    idx_t pos_in_row = n - prev_total - 1ull;      /* zero‑based within row */
    seq_t result = u_a[pos_in_row];

    /* Clean up. */
    free(u_a); free(v_a); free(u_b); free(v_b);

    return result;
}
