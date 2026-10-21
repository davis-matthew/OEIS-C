/*
 * A225478  Triangle read by rows, 4^k*s_4(n, k) where s_m(n, k) are the
 *          Stirling-Frobenius cycle numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where: T(0,0) = 1 T(n,0) = (4n-1) * T(n-1,0)
 *            T(n,k) = 4 * T(n-1,k-1) + (4n-1) * T(n-1,k) for 1 ≤ k ≤ n The
 *            pair (r,k) is obtained by reading the triangle row‑by‑row: r =
 *            floor( (sqrt(8n+1)-1)/2 ), k = n - r(r+1)/2 . The sequence a(n)
 *            is the triangle read by rows
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 37, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find row r and column k such that
       n = r*(r+1)/2 + k , 0 <= k <= r */
    idx_t lo = 0;
    idx_t hi = n;
    idx_t r = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t val = mid * (mid + 1) / 2;
        if (val == n) {
            r = mid;
            break;
        }
        if (val < n) {
            r = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    idx_t k = n - r * (r + 1) / 2;

    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Allocate two rows of size r+1 */
    size_t sz = r + 1;
    seq_t *prev = NULL;
    seq_t *curr = NULL;

#ifdef __cplusplus
    prev = new seq_t[sz];
    curr = new seq_t[sz];
#else
    prev = (seq_t *)malloc(sz * sizeof(seq_t));
    curr = (seq_t *)malloc(sz * sizeof(seq_t));
#endif

    prev[0] = (seq_t)1;

    for (idx_t row = 1; row <= r; ++row) {
        seq_t factor = (seq_t)(4 * (seq_t)row - 1);   /* 4*row-1 */

        curr[0] = factor * prev[0];
        for (idx_t col = 1; col < row; ++col) {
            curr[col] = 4 * prev[col - 1] + factor * prev[col];
        }
        curr[row] = 4 * prev[row - 1];

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];

#ifdef __cplusplus
    delete[] curr;
    delete[] prev;
#else
    free(curr);
    free(prev);
#endif

    return result;
}
