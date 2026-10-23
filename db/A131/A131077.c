/*
 * A131077  Antidiagonal sums of triangular array T: T(j,1) = 1 for ((j-1) mod
 *          8) < 4, else 0; T(j,k) = T(j-1,k-1) + T(j,k-1) for 2 <= k <= j.
 * https://oeis.org/A131077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} T(k, n-k+1) where T(j,1) = 1 if ((j-1) mod 8)
 *            < 4 else 0, and T(j,k) = T(j-1,k-1) + T(j,k-1) for 2 ≤ k ≤ j
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for 0, return 0 */
        return (seq_t)0;

    /* allocate arrays of size n+1 (index 0 unused) */
    seq_t *a_arr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *prev   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *curr   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));

    if (!a_arr || !prev || !curr) { /* allocation failure */
        /* safe fallback – return 0 */
        free(prev); free(curr); free(a_arr);
        return (seq_t)0;
    }

    /* compute rows j = 1 .. n */
    for (idx_t j = 1; j <= n; ++j) {
        curr[1] = (((j - 1) % 8) < 4) ? 1 : 0;
        for (idx_t k = 2; k <= j; ++k)
            curr[k] = prev[k - 1] + curr[k - 1];

        /* add current row to the antidiagonal sums a[ant] */
        for (idx_t k = 1; k <= j; ++k) {
            idx_t ant = j + k - 1;            /* antidiagonal index */
            if (ant <= n)
                a_arr[ant] += curr[k];
        }

        /* prepare for next row: swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = a_arr[n];

    free(prev);
    free(curr);
    free(a_arr);
    return result;
}
