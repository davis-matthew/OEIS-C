/*
 * A142462  Triangle read by rows: T(n,k) (1<=k<=n) given by T(n, 1) = T(n,n)
 *          = 1, otherwise T(n, k) = (m*n-m*k+1)*T(n-1,k-1) +
 *          (m*k-m+1)*T(n-1,k), where m = 7.
 * https://oeis.org/A142462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   * T(1,1)=1, T(n,1)=T(n,n)=1, * T(n,k) = (m*n - m*k + 1) *
 *            T(n-1,k-1) + (m*k - m + 1) * T(n-1,k), m=7 * a(n) is the
 *            triangle read by rows, that is a(n)=T(r,k) where r is the * row
 *            containing the n-th term and k the column within that row
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 40, u64 n <= 109,
 *            u128 n <= 285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t M = (seq_t)7U;

seq_t a(idx_t n)
{
    /* Handle the first term directly. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Find the row r:  r(r-1)/2 < n <= r(r+1)/2 */
    idx_t r = 1;
    for (;; ++r) {
        seq_t cum = (seq_t)r * ((seq_t)r + (seq_t)1) / (seq_t)2;
        if (cum >= (seq_t)n)
            break;
    }

    /* Column index within the row (1‑based). */
    idx_t k = n - ((r - 1) * r) / 2;   /* k in [1, r] */

    /* Allocate two rows of the triangle. Use explicit cast for C++ safety. */
    seq_t *prev = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (prev == NULL || curr == NULL) {
        /* Allocation failed – return zero (unlikely in tests). */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Initialize the first row (n = 1). */
    prev[0] = (seq_t)1;          /* T(1,1) = 1 */

    /* Build rows up to r. */
    for (idx_t i = 2; i <= r; ++i) {
        curr[0] = (seq_t)1;
        curr[i - 1] = (seq_t)1; /* left and right borders */

        for (idx_t j = 1; j <= i - 2; ++j) {   /* k = j+1 */
            seq_t coeff_left  = (seq_t)M * (seq_t)i - (seq_t)M * (seq_t)(j + 1) + (seq_t)1;
            seq_t coeff_right = (seq_t)M * (seq_t)(j + 1) - (seq_t)M + (seq_t)1;
            curr[j] = coeff_left  * prev[j - 1] + coeff_right * prev[j];
        }

        if (i == r) {
            seq_t result = curr[k - 1];
            free(prev);
            free(curr);
            return result;
        }

        /* Continue to next row: current becomes previous. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Should never reach here. Clean up. */
    free(prev);
    free(curr);
    return (seq_t)0;
}
