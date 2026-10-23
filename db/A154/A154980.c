/*
 * A154980  Triangle T(n, k, m) = coefficients of p(x, n, m) where p(x,n,m) =
 *          (x+1)*p(x, n-1, m) + 2^(m+n-1) *x*p(x, n-2, m) and m=1, read by
 *          rows.
 * https://oeis.org/A154980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, k) where T satisfies T(n,k) = T(n-1,k) +
 *            T(n-1,k-1) + 2^n * T(n-2,k-1), T(0,0)=1, T(1,0)=T(1,1)=1, and
 *            rows are concatenated: a(0)=1, a(1)=T(1,0), a(2)=T(1,1),
 *            a(3)=T(2,0), a(4)=T(2,1), a(5)=T(2,2), …
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 69, u64 n <= 126,
 *            u128 n <= 262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) that contains the flattened index n.
       The cumulative number of terms up to and including row r is
       (r+1)(r+2)/2.  Find the smallest r with this > n. */
    idx_t r = 0;
    idx_t cumulative = 1;  /* (r+1)(r+2)/2 for r=0 */
    while (cumulative <= n) {
        ++r;
        cumulative = (r + 1) * (r + 2) / 2;
    }
    idx_t prev_cumulative = cumulative - (r + 1);
    idx_t k = n - prev_cumulative;   /* position within the row */

    /* For the first two rows the value is always 1. */
    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)1;

    /* Allocate three rows of size r+1 (the maximum needed). */
    size_t sz = (size_t)(r + 1);
    seq_t *row_nm2 = (seq_t*)calloc(sz, sizeof(seq_t));  /* row n‑2   */
    seq_t *row_nm1 = (seq_t*)calloc(sz, sizeof(seq_t));  /* row n‑1   */
    seq_t *row_n   = (seq_t*)calloc(sz, sizeof(seq_t));  /* current   */

    if (!row_nm2 || !row_nm1 || !row_n) {
        free(row_nm2); free(row_nm1); free(row_n);
        return (seq_t)0;   /* allocation failure – unlikely for n≤10000 */
    }

    /* Base rows */
    row_nm2[0] = 1;                   /* T(0,0) */
    row_nm1[0] = row_nm1[1] = 1;        /* T(1,0)=T(1,1)=1 */

    /* 2^i factor for row i (starting with i = 2).  Initialise to 2 so
       that after the first multiplication we obtain 2^2 for row 2. */
    seq_t pow2 = (seq_t)2;

    for (idx_t i = 2; i <= r; ++i) {
        pow2 = pow2 * (seq_t)2;   /* now 2^i */

        row_n[0] = 1;
        row_n[i] = 1;
        for (idx_t j = 1; j < i; ++j) {
            row_n[j] = row_nm1[j] + row_nm1[j - 1] + pow2 * row_nm2[j - 1];
        }

        /* Rotate buffers for the next iteration. */
        seq_t *tmp = row_nm2;
        row_nm2   = row_nm1;
        row_nm1   = row_n;
        row_n     = tmp;   /* reuse buffer */
    }

    seq_t result = row_nm1[k];

    free(row_nm2);
    free(row_nm1);
    free(row_n);

    return result;
}
