/*
 * A208660  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208904; see the Formula section.
 * https://oeis.org/A208660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(n,x) = u(n-1,x) + 2x * v(n-1,x) v(n,x) = u(n-1,x) + (x+1) *
 *            v(n-1,x) + 1 u(1,x) = v(1,x) = 1 a(k) is the k‑th coefficient of
 *            the triangular array of the polynomials u(n,x), listed row by
 *            row, coefficients from constant term to highest degree
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* out of range guard */

    /* Determine the row that contains the n-th coefficient. */
    double d  = sqrt((double)(8.0 * (double)n + 1.0));
    idx_t row = (idx_t)ceil((d - 1.0) / 2.0);

    /* Previous row arrays: u_prev and v_prev. */
    seq_t *u_prev = (seq_t*)malloc(row * sizeof(seq_t));
    seq_t *v_prev = (seq_t*)malloc(row * sizeof(seq_t));
    if (!u_prev || !v_prev) return (seq_t)0;   /* allocation failure */

    /* base row 1 */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    if (n == 1) {
        free(u_prev);
        free(v_prev);
        return (seq_t)1;
    }

    idx_t prev_len = 1;
    idx_t cumulative = 1;          /* coefficients processed so far */

    for (idx_t r = 2; r <= row; ++r) {
        seq_t *u_new = (seq_t*)malloc(r * sizeof(seq_t));
        seq_t *v_new = (seq_t*)malloc(r * sizeof(seq_t));
        if (!u_new || !v_new) {
            free(u_prev); free(v_prev);
            free(u_new); free(v_new);
            return (seq_t)0;
        }

        for (idx_t k = 0; k < r; ++k) {
            seq_t uprev = (k < prev_len) ? u_prev[k] : (seq_t)0;
            seq_t shiftV = (k >= 1 && k - 1 < prev_len) ? v_prev[k - 1] : (seq_t)0;
            u_new[k] = uprev + shiftV * (seq_t)2;

            seq_t vprev_k  = (k < prev_len) ? v_prev[k] : (seq_t)0;
            seq_t vprev_k1 = (k >= 1 && k - 1 < prev_len) ? v_prev[k - 1] : (seq_t)0;
            v_new[k] = uprev + vprev_k + vprev_k1 + ((k == 0) ? (seq_t)1 : (seq_t)0);
        }

        /* Does the requested coefficient lie in this row? */
        if (n <= cumulative + r) {
            idx_t pos = n - cumulative - 1;   /* zero–based position */
            seq_t result = u_new[pos];
            free(u_prev); free(v_prev);
            free(u_new); free(v_new);
            return result;
        }

        /* Prepare for next row */
        free(u_prev); free(v_prev);
        u_prev = u_new;
        v_prev = v_new;
        prev_len = r;
        cumulative += r;
    }

    /* Should never reach here */
    free(u_prev); free(v_prev);
    return (seq_t)0;
}
