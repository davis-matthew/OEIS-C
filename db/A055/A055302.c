/*
 * A055302  Triangle of number of labeled rooted trees with n nodes and k
 *          leaves, n >= 1, 1 <= k <= n.
 * https://oeis.org/A055302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, eigen
 * Formula:   a(n) = (m!/k!) * S(m‑1, m‑k) * where n is the linear index into
 *            the triangle, * m is the row such that m(m+1)/2 >= n, * and k =
 *            n – (m-1)m/2 (1‑based column). * S is the Stirling number of the
 *            second kind
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 57, u64 n <= 156,
 *            u128 n <= 412, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range but safe */

    /* find row m */
    idx_t m = 0;
    while (m * (m + 1) / 2 < n) ++m;

    /* column k */
    idx_t k = n - m * (m - 1) / 2;
    if (k < 1 || k > m) return (seq_t)0;  /* safety, should not happen */

    /* compute factorial ratio m!/k! = product_{i=k+1}^{m} i */
    seq_t fact_ratio = (seq_t)1;
    for (idx_t i = k + 1; i <= m; ++i) {
        fact_ratio *= (seq_t)i;
    }

    /* n for Stirling: n_s = m - 1, k_s = m - k */
    idx_t n_s = m - 1;
    idx_t k_s = m - k;
    if (k_s > n_s) return (seq_t)0;       /* should not happen */

    /* compute S(n_s, k_s) with 1‑D DP */
    seq_t *S = (seq_t *)calloc((size_t)(n_s + 1), sizeof(seq_t));
    if (S == NULL) return (seq_t)0;       /* allocation failure, unlikely */

    S[0] = (seq_t)1;                       /* S(0,0)=1 */
    for (idx_t i = 1; i <= n_s; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            S[j] = S[j] * (seq_t)j + S[j - 1];
        }
        S[0] = (seq_t)0;
    }

    seq_t Stirling_val = S[k_s];
    free(S);

    if (Stirling_val == (seq_t)0) return (seq_t)0;

    /* result = fact_ratio * Stirling_val */
    return fact_ratio * Stirling_val;
}
