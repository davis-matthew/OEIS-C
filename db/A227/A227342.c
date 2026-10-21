/*
 * A227342  Expansion of (1 - t)*(1 + t)^x.
 * https://oeis.org/A227342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   The terms a(n) are the flattened triangle of T(r,k) =
 *            Stirling1(r,k) – r * Stirling1(r-1,k), where Stirling1(r,k) is
 *            the signed Stirling number of the first kind. For a given n, let
 *            r be the largest integer with r·(r+1)/2 ≤ n. Then k = n –
 *            r·(r+1)/2 (0 ≤ k ≤ r) and a(n) = T(r,k)
 * Range:     i8 n <= 21, i16 n <= 45, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* n = 0 returns the first entry of the triangle */
    if (n == 0) return (seq_t)1;

    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    /* Adjust for rounding errors */
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n) r++;
    while (((__int128)r * (r + 1)) / 2 > n) r--;

    idx_t k = n - (idx_t)((__int128)r * (r + 1) / 2);

    /* Allocate two rows of Stirling numbers (signed).  Size r+1 is enough. */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        /* Allocation failed – return 0 as a safe fallback. */
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;
    }

    /* Initialize row 0 */
    for (idx_t i = 0; i <= r; ++i) prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    seq_t result = (seq_t)0;
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)0;
        for (idx_t j = 1; j <= i; ++j) {
            /* s(i,j) = s(i-1,j-1) - (i-1)*s(i-1,j) */
            curr[j] = prev[j - 1] - (seq_t)(i - 1) * prev[j];
        }
        if (i == r) {
            /* T(r,k) = s(r,k) - r * s(r-1,k) */
            seq_t s_r_k   = curr[k];
            seq_t s_rm1_k = (k <= r - 1) ? prev[k] : (seq_t)0;
            result = s_r_k - (seq_t)r * s_rm1_k;
        }
        /* Prepare for next iteration: prev = curr */
        for (idx_t j = 0; j <= i; ++j) prev[j] = curr[j];
    }

    free(prev);
    free(curr);
    return result;
}
