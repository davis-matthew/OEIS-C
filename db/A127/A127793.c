/*
 * A127793  Inverse of number triangle t(n,k) = 1/floor((n+2)/2) if k <= n <=
 *          2*k, 0 otherwise.
 * https://oeis.org/A127793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9930, i16 n <= 9930, i32 n <= 9930, i64 n <= 9930,
 *            i128 n <= 9930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the number triangle t(n,k) (OEIS A127793).
 * The triangle t(n,k) is defined by t(n,k)=1/floor((n+2)/2) if
 * ceil(n/2) ≤ k ≤ n, and 0 otherwise.  Its inverse B(n,k) satisfies
 *   B(n,k) = floor((n+2)/2) * [n==k] - Σ_{m=max(ceil(n/2),k)}^{n-1} B(m,k)
 * The sequence a(n) is the row‑major flattening of B(n,k).
 * The implementation keeps a cache of the inverse triangle and
 * computes new rows on demand.  Works for any seq_t and idx_t
 * combination provided by the build. */

#include <stdlib.h>

/* static cache for the inverse triangle */
static seq_t **inv_rows = NULL;
static idx_t inv_rows_count = 0;
static idx_t inv_rows_cap = 0;

/* Ensure inv_rows contains rows up to index 'up_to' (inclusive). */
static void ensure_inv_rows(idx_t up_to) {
    if (inv_rows == NULL) {
        inv_rows_cap = 2;
        inv_rows = (seq_t**)malloc(inv_rows_cap * sizeof(seq_t*));
    }
    while (inv_rows_count <= up_to) {
        if (inv_rows_count == inv_rows_cap) {
            inv_rows_cap *= 2;
            inv_rows = (seq_t**)realloc(inv_rows, inv_rows_cap * sizeof(seq_t*));
        }
        idx_t i = inv_rows_count;          /* row index to compute */
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));

        idx_t m = (i + 2) / 2;              /* floor((i+2)/2)   */
        idx_t start = (i + 1) / 2;          /* ceil(i/2)        */

        for (idx_t k = 0; k < i; ++k) {
            seq_t sum = 0;
            idx_t j_start = (start > k) ? start : k;
            for (idx_t j = j_start; j < i; ++j) {
                sum += inv_rows[j][k];
            }
            row[k] = -sum;
        }
        row[i] = (seq_t)m;                 /* diagonal element */
        inv_rows[i] = row;
        ++inv_rows_count;
    }
}

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t c = n - r * (r + 1) / 2;          /* column within the row */

    ensure_inv_rows(r);                     /* compute necessary rows */
    return inv_rows[r][c];
}
