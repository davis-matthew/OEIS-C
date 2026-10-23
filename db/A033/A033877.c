/*
 * A033877  Triangular array read by rows associated with Schroeder numbers:
 *          T(1,k) = 1; T(n,k) = 0 if k < n; T(n,k) = T(n,k-1) + T(n-1,k-1) +
 *          T(n-1,k).
 * https://oeis.org/A033877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) is the entry T(c,k) of a triangular array read by rows
 *            where T(1,k) = 1, T(n,k) = 0 if n < k, T(n,k) =
 *            T(n,k-1)+T(n-1,k-1)+T(n-1,k) otherwise. For each n≥1 let k be
 *            the unique row with k(k+1)/2 ≥ n, then c = n - k(k-1)/2 and a(n)
 *            = T(c,k)
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 149, u64 n <= 461,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find row k such that k(k+1)/2 >= n */
    idx_t k = 1;
    idx_t t = 1;                       /* k(k+1)/2 so far */
    while (t < n) {
        ++k;
        t += k;
    }
    idx_t c = n - (t - k);             /* column index (1‑based) */

    /* build the triangular array up to row k */
    /* we keep two arrays: prev (row k-1), cur (row k) */
    seq_t *prev = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (!prev || !cur) return (seq_t)0; /* fallback on allocation error */

    prev[1] = 1;                        /* row 1: T(1,1)=1 */

    for (idx_t row = 2; row <= k; ++row) {
        cur[1] = 1;                    /* T(1,row)=1 */

        for (idx_t col = 2; col <= row; ++col) {
            seq_t term1 = (col <= row - 1) ? prev[col] : 0; /* T(col,row-1)   */
            seq_t term2 = (col - 1 <= row - 1) ? prev[col - 1] : 0; /* T(col-1,row-1) */
            seq_t term3 = cur[col - 1];                       /* T(col-1,row)   */
            cur[col] = term1 + term2 + term3;
        }

        /* swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t ret = prev[c];

    free(prev);
    free(cur);
    return ret;
}
