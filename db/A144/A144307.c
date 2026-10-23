/*
 * A144307  Triangle read by rows: prime numbers p(n) along left edge (n, 1)
 *          and p(n - 1) along right edge (n, n), with (n, k) = rad((n - 1, k
 *          - 1) + (n - 1, k)) for 1 < k < n when n > 2.
 * https://oeis.org/A144307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is read from a triangle T(n,k) (1 ≤ k ≤ n) defined by
 *            T(1,1) = 2, T(2,1) = p(2), T(2,2) = p(1), left edge: T(n,1) =
 *            p(n), right edge: T(n,n) = p(n-1) (for n>1), interior: T(n,k) =
 *            rad( T(n‑1,k‑1) + T(n‑1,k) ) (1<k<n, n>2) where p(i) is the i‑th
 *            prime and rad(x) is the product of distinct pri...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   get_prime, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/rad.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0, but safe fallback */

    /* Find the row r such that r(r-1)/2 < n <= r(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1u)) / 2u < n) r++;

    /* Position k within the row (1‑based) */
    idx_t k = n - (r * (r - 1u)) / 2u;

    /* Allocate two working arrays of maximal length r */
    seq_t *prev = (seq_t *)malloc(r * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(r * sizeof(seq_t));
    if (!prev || !curr) { /* Allocation failed – return 0 as a safe fail */
        free(prev); free(curr);
        return 0;
    }

    /* Row 1 */
    prev[0] = (seq_t)get_prime(1u);
    if (r == 1u) {
        seq_t result = prev[0];
        free(prev); free(curr);
        return result;
    }

    /* Row 2 */
    curr[0] = (seq_t)get_prime(2u);
    curr[1] = (seq_t)get_prime(1u);

    /* If only two rows are required, answer directly */
    if (r == 2u) {
        seq_t result = curr[k - 1];
        free(prev); free(curr);
        return result;
    }

    /* Prepare for iterative construction of rows >= 3 */
    seq_t *row_prev = curr;   /* current row (starting with row 2) */
    seq_t *row_curr = prev;   /* buffer for new row */

    for (idx_t row = 3u; row <= r; ++row) {
        /* leftmost element */
        row_curr[0] = (seq_t)get_prime(row);

        /* interior elements */
        for (idx_t i = 1u; i < row - 1u; ++i) {
            seq_t sum = row_prev[i - 1] + row_prev[i];
            row_curr[i] = rad((idx_t)sum);
        }

        /* rightmost element */
        row_curr[row - 1] = (seq_t)get_prime(row - 1u);

        /* swap row_prev and row_curr for next iteration */
        seq_t *tmp = row_prev;
        row_prev = row_curr;
        row_curr = tmp;
    }

    seq_t result = row_prev[k - 1];
    free(prev); free(curr);
    return result;
}
