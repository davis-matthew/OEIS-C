/*
 * A204172  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of (f(i,j)), where
 *          f(i,j)=(1 if max(i,j) is odd, and 0 otherwise) as in A204171.
 * https://oeis.org/A204172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 125, i16 n <= 389, i32 n <= 1250, i64 n <= 4800,
 *            i128 n <= 9679, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence-based computation of OEIS A204172.
    The sequence is obtained from coefficients of the
    characteristic polynomial of the n-th principal submatrix
    of the matrix f(i,j) = 1 if max(i,j) is odd, 0 otherwise.
    The recurrence for the polynomial coefficients is derived
    from block matrix identities:

        Even n :   p_n(x) = -x·p_{n-1}(x)
                   q_n(x) = -x·q_{n-1}(x) + p_{n-1}(x)

        Odd  n :   p_n(x) = (1−x)·p_{n-1}(x) − q_{n-1}(x)
                   q_n(x) = p_n(x) + x·p_{n-1}(x) − x·q_{n-1}(x)

    Here   p_k(x) = det(A_k - xI)    (coefficients of the
    characteristic polynomial of size k)  and
    q_k(x) = 1ᵀ·adj(A_k - xI)·1.  Only coefficient
    arithmetic is required; no division or floating‑point
    operations are used.

    The function returns the n‑th coefficient in the
    flattened array of all rows (row 1 has length 2,
    row 2 length 3, etc.).  It works for any idx_t, seq_t
    provided by the build.  No static memory is used
    beyond the temporary polynomials.  The routine
    runs in O(r²) time where r ≈ √(2n) ≤ 141 for n ≤ 10⁴.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /*-------------------- Find row r and offset --------------------*/
    idx_t r = 0;
    /*  cumulative terms up to row r = r(r+3)/2  */
    while ( (r * (r + 3)) / 2 < n ) r++;

    idx_t prev_count = ( (r - 1) * ( (r - 1) + 3 ) ) / 2;
    idx_t offset = n - prev_count - 1;   /* 0‑based index inside row r */

    /*-------------------- Allocate polynomial arrays --------------------*/
    /* Max degree needed is r.  Allocate arrays large enough for all steps. */
    seq_t *prev_p = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *prev_q = (seq_t *)calloc(r + 1, sizeof(seq_t));
    /* Initially for n = 1: p(x) = 1 - x, q(x) = 1   (constant) */
    prev_p[0] = 1;          /* coefficient of x⁰  */
    prev_p[1] = -1;         /* coefficient of x¹  */
    prev_q[0] = 1;          /* q₁(x) = 1 */

    /*-------------------- Build up to row r --------------------*/
    for (idx_t i = 2; i <= r; ++i) {
        size_t sz = i + 1;                /* new degree = i */
        seq_t *next_p = (seq_t *)calloc(sz, sizeof(seq_t));
        seq_t *next_q = (seq_t *)calloc(sz, sizeof(seq_t));

        if (i & 1) { /* odd i */
            /* pᵢ = (1 - x)·pᵢ₋₁ - qᵢ₋₁ */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (k <= i - 1) val += prev_p[k];
                if (k >= 1)     val -= prev_p[k - 1];
                if (k <= i - 1) val -= prev_q[k];
                next_p[k] = val;
            }
            /* qᵢ = pᵢ + x·pᵢ₋₁ - x·qᵢ₋₁ */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = next_p[k];
                if (k >= 1) val += prev_p[k - 1];
                if (k >= 1) val -= prev_q[k - 1];
                next_q[k] = val;
            }
        } else { /* even i */
            /* pᵢ = -x·pᵢ₋₁ */
            next_p[0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                next_p[k] = -prev_p[k - 1];
            }
            /* qᵢ = -x·qᵢ₋₁ + pᵢ₋₁ */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (k <= i - 1) val += prev_p[k];
                if (k >= 1)     val -= prev_q[k - 1];
                next_q[k] = val;
            }
        }
        /* Free previous polynomials and advance */
        free(prev_p);
        free(prev_q);
        prev_p = next_p;
        prev_q = next_q;
    }

    /*-------------------- Return requested coefficient --------------------*/
    seq_t result = prev_p[offset];
    free(prev_p);
    free(prev_q);
    return result;
}
