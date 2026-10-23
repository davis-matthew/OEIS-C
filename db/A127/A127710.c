/*
 * A127710  Inverse of the triangle A(n,k)=if(k<=n,if(n<=2k,1/(n+1),0),0).
 * https://oeis.org/A127710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the linear index of the inverse of the triangle *
 *            T(n,k)=1/(n+1) for k<=n<=2k, 0 otherwise. * Let W[n][k] =
 *            V[n][k]/(k+1) where V is the inverse triangle. * Then W[n][n] =
 *            1 and for 0 <= k < n: * W[n][k] = - Σ_{m=k+1}^{min(n, 2k)}
 *            W[n][m]. * For any index n, let r be the row such that r(r...
 * Range:     i8 n <= 56, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r containing index n */
    idx_t low = 0, high = n;
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri <= n) {
            /* Check if mid is the right row */
            idx_t tri_next = ((mid + 1) * (mid + 2)) / 2;
            if (tri_next > n) {
                r = mid;
                break;
            } else {
                low = mid + 1;
            }
        } else {
            high = mid - 1;
        }
    }

    /* column within the row */
    idx_t c = n - (r * (r + 1)) / 2;

    /* Allocate arrays for W and suffix sums */
    seq_t *W = (seq_t *)malloc((r + 2) * sizeof(seq_t));
    seq_t *suffix = (seq_t *)malloc((r + 2) * sizeof(seq_t));
    if (!W || !suffix) {
        /* In the unlikely event of allocation failure, return 0 */
        if (W) free(W);
        if (suffix) free(suffix);
        return (seq_t)0;
    }

    suffix[r + 1] = (seq_t)0;  /* suffix[r+1] = 0 */

    /* Compute W downward from r to 0 */
    for (idx_t i = r; ; --i) {
        idx_t R = (2 * i <= r) ? (2 * i) : r;  /* R = min(n, 2*i) */
        seq_t sumRange = suffix[i + 1] - suffix[R + 1];
        seq_t val = ((i == r) ? (seq_t)1 : (seq_t)0) - sumRange;
        W[i] = val;
        suffix[i] = val + suffix[i + 1];
        if (i == 0) break;
    }

    /* Result is (c+1) * W[c] */
    seq_t result = (seq_t)(c + 1) * W[c];

    free(W);
    free(suffix);
    return result;
}
