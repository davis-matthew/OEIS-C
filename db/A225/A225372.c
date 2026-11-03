/*
 * A225372  Triangle read by rows: T(n,k) (1 <= k <= n) given by T(n, 1) =
 *          T(n,n) = 1, otherwise T(n, k) = (m*n-m*k+1)*T(n-1,k-1) +
 *          (m*k-m+1)*T(n-1,k), where m = -2.
 * https://oeis.org/A225372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(row,col) from the triangle with recurrence:
 *            T(n,1)=T(n,n)=1,
 *            T(n,k)=(-2*n+2*k+1)*T(n-1,k-1)+(-2*k+3)*T(n-1,k). The sequence
 *            a(n) is the triangle read by rows (first row n=1, second row
 *            n=2, …)
 * Range:     i8 n <= 59, i16 n <= 179, i32 n <= 611, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Validated: standard, pass, 82 terms through n = 1241 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th term:
       n ≤ r(r+1)/2 < n+1  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Row 1 is trivial. */
    if (r == 1)
        return (seq_t)1;

    /* Allocate two rows (1‑indexed).  Size r+1 suffices for all rows
       up to r. */
    seq_t *prev = mem_new(seq_t, (r + 1));
    seq_t *curr = mem_new(seq_t, (r + 1));
    if (!prev || !curr) {
        if (prev) mem_free(prev);
        if (curr) mem_free(curr);
        return (seq_t)0;   /* allocation failure: return zero */
    }

    /* Initial row (n = 1). */
    prev[1] = (seq_t)1;

    /* Build rows 2 … r. */
    for (idx_t i = 2; i <= r; ++i) {
        curr[1] = curr[i] = (seq_t)1;
        for (idx_t k = 2; k < i; ++k) {
            /* Compute the coefficients directly. */
            seq_t coeff1 = -((seq_t)2 * i) + ((seq_t)2 * k) + 1;  /* -2i + 2k + 1 */
            seq_t coeff2 = -((seq_t)2 * k) + 3;                   /* -2k + 3 */
            curr[k] = coeff1 * prev[k - 1] + coeff2 * prev[k];
        }

        /* Prepare for next iteration: swap prev and curr. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Column (1‑based) of the n‑th term within row r. */
    idx_t col = n - (r * (r - 1) / 2);

    seq_t result = prev[col];

    mem_free(prev);
    mem_free(curr);

    return result;
}
