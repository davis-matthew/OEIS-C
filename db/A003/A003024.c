/*
 * A003024  Number of acyclic digraphs (or DAGs) with n labeled nodes.
 * https://oeis.org/A003024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1)*C(n,k)*2^(k*(n-k))*a(n-k), a(0) =
 *            1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   a003024_pow2, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/a003024_pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    seq_t *dp = alloc_zero(n + 1);
    seq_t *C = alloc_zero(n + 1);   /* Pascal row C(i, k) */
    dp[0] = 1;
    C[0] = 1;

    for (idx_t i = 1; i <= n; i++) {
        /* update Pascal row to row i */
        for (idx_t k = i; k >= 1; k--)
            C[k] += C[k - 1];

        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; k <= i; k++) {
            seq_t term = C[k] * a003024_pow2(k * (i - k)) * dp[i - k];
            if (k & 1) pos += term;
            else neg += term;
        }
        dp[i] = pos - neg;
    }

    seq_t result = dp[n];
    free(dp);
    free(C);
    return result;
}
