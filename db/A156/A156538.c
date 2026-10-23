/*
 * A156538  Triangle T(n, k, q) = e(n, k, q) + e(n, n-k+1, q), where e(n, k,
 *          q) = ((1 - (-q)^k)/(1 + q))*e(n-1, k, q) + (-q)^(k-1)*e(n-1, k-1,
 *          q), e(n, 0, q) = e(n, n, q) = 1, and q = 3, read by rows.
 * https://oeis.org/A156538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = e(n,k)+e(n,n-k+1), q = 3 e(1,0)=0, e(1,1)=1, e(n,0)=0,
 *            e(n,n)=1 e(n,k) = ((1-(-q)^k)/(1+q))*e(n-1,k) +
 *            (-q)^(k-1)*e(n-1,k-1) (1<=k<=n-1) a(n) is the n‑th element read
 *            by rows of T
 * Range:     i8 n <= 11, i16 n <= 16, i32 n <= 29, i64 n <= 56, i128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    idx_t total = 0;
    while (total < n) {
        ++r;
        total += r;
    }

    idx_t prev_total = (r - 1) * r / 2;      /* terms before row r */
    idx_t k0 = n - prev_total;               /* 1 <= k0 <= r */

    /* powers of (-3) up to r */
    seq_t *pow_neg3 = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    pow_neg3[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i)
        pow_neg3[i] = pow_neg3[i - 1] * (seq_t)(-3);

    /* two rows: previous and current */
    seq_t *e_prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *e_cur  = (seq_t *)malloc((r + 1) * sizeof(seq_t));

    /* row 1 */
    e_prev[0] = (seq_t)0;
    e_prev[1] = (seq_t)1;
    if (r == 1) {
        seq_t ans = e_prev[1] + e_prev[1];
        free(e_prev);
        free(e_cur);
        free(pow_neg3);
        return ans;
    }

    /* compute successive rows up to r */
    for (idx_t cur = 2; cur <= r; ++cur) {
        e_cur[0] = (seq_t)0;
        e_cur[cur] = (seq_t)1;
        for (idx_t k = 1; k < cur; ++k) {
            seq_t A = (seq_t)(1 - pow_neg3[k]) / (seq_t)4;   /* (1-(-3)^k)/(1+3) */
            seq_t B = pow_neg3[k - 1];                       /* (-3)^(k-1)          */
            e_cur[k] = A * e_prev[k] + B * e_prev[k - 1];
        }
        /* swap row buffers */
        seq_t *tmp = e_prev;
        e_prev = e_cur;
        e_cur = tmp;
    }

    seq_t result = e_prev[k0] + e_prev[r - k0 + 1];

    free(e_prev);
    free(e_cur);
    free(pow_neg3);
    return result;
}
