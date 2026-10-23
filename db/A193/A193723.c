/*
 * A193723  Mirror of the fusion triangle A193722.
 * https://oeis.org/A193723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) defined by
 *            T(n,k) = T(n-1,k-1) + 3*T(n-1,k) with initial entries T(0,0) =
 *            1, T(1,0) = 2, T(1,1) = 1. a(n) is read row‑wise, i.e.
 *            a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r and column k such that n = r(r+1)/2 + k,
       with 0 ≤ k ≤ r.  r is the largest integer with r(r+1)/2 ≤ n. */
    idx_t r;
    /* Compute using floating point and fix possible rounding errors */
    long double val = 8.0L * (long double)n + 1.0L;
    r = (idx_t)((sqrtl(val) - 1.0L) / 2.0L);
    /* Correct r if necessary */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Handle special rows directly */
    if (r == 0) {
        return (seq_t)1;
    }
    if (r == 1) {
        return (k == 0) ? (seq_t)2 : (seq_t)1;
    }

    /* Allocate storage for two consecutive rows */
    seq_t *prev = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!prev || !cur) {
        /* Allocation failed – return zero (should not happen in tests) */
        if (prev) free(prev);
        if (cur)  free(cur);
        return (seq_t)0;
    }

    /* Row 1: 2, 1 */
    prev[0] = (seq_t)2;
    prev[1] = (seq_t)1;

    /* Build rows 2 … r */
    for (idx_t m = 2; m <= r; ++m) {
        /* Compute cur[0] = 3 * prev[0]  (for m ≥ 2 this follows recurrence) */
        cur[0] = prev[0] * (seq_t)3;
        /* Middle elements */
        for (idx_t j = 1; j < m; ++j) {
            cur[j] = prev[j - 1] + prev[j] * (seq_t)3;
        }
        /* Last element */
        cur[m] = prev[m - 1];
        /* Swap pointers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t result = prev[k];

    free(prev);
    free(cur);
    return result;
}
