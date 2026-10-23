/*
 * A166961  Triangle T(n,k) read by rows: T(n,k) = (m*n - m*k + 1)*T(n - 1, k
 *          - 1) + k*(m*k - (m - 1))*T(n - 1, k) where m = 2.
 * https://oeis.org/A166961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangular array T(n,k) with * T(n,k) =
 *            (2*n - 2*k + 1)*T(n-1,k-1) + k*(2*k - 1)*T(n-1,k), * where
 *            T(1,1)=1 and T(n,k)=0 when k<1 or k>n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 48, u64 n <= 110,
 *            u128 n <= 284, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for pre‑computed rows of the triangle
 * tri[i][k] holds T(i,k) for 1 ≤ k ≤ i.  */
static seq_t **tri = NULL;      /* array of row pointers, using 1‑based row indices   */
static idx_t max_rows = 0;      /* largest row that has been computed so far     */

/* ensure that rows 1..r of the triangle are computed */
static void ensure_rows_up_to(idx_t r)
{
    if (r <= max_rows) return;

    /* enlarge the pointer array to hold rows up to r */
    tri = (seq_t **)realloc(tri, (r + 1) * sizeof(seq_t *));
    if (tri == NULL) abort();  /* allocation failure */

    /* compute the missing rows, starting from max_rows+1 */
    for (idx_t i = max_rows + 1; i <= r; ++i) {
        /* each row has i+1 elements (indices 0..i); index 0 is unused */
        tri[i] = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (tri[i] == NULL) abort();  /* allocation failure */

        if (i == 1) {                 /* base case */
            tri[1][1] = (seq_t)1;
            continue;
        }

        /* compute T(i,k) for all k = 1 .. i */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t left  = 0;
            seq_t right = 0;

            if (k > 1) {
                /* (2*i - 2*k + 1) * T(i-1,k-1) */
                seq_t coeff = (seq_t)(2 * i - 2 * k + 1);
                left = coeff * tri[i - 1][k - 1];
            }
            if (k < i) {
                /* k*(2*k - 1) * T(i-1,k) */
                seq_t coeff = (seq_t)k * (seq_t)(2 * k - 1);
                right = coeff * tri[i - 1][k];
            }
            tri[i][k] = left + right;
        }
    }
    max_rows = r;
}

/* return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n) {
        ++r;
    }

    /* position k within that row (1‑based) */
    idx_t k = n - (idx_t)((r - 1) * r / 2);

    ensure_rows_up_to(r);

    return tri[r][k];
}
