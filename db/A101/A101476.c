/*
 * A101476  Sum[k=0..n, Sum[i>=0, C(2n,n-i(k+2))-C(2n,n+i(k+2)+k+1) ]].
 * https://oeis.org/A101476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum[k=0..n, Sum[i>=0, C(2n,n-i(k+2))-C(2n,n+i(k+2)+k+1)
 *            ]]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Pre‑compute the row binomial coefficients C(2n, k) for k=0..2n */
    idx_t two_n = n + n;
    size_t len = (size_t)(two_n + 1);
    seq_t *comb = (seq_t *)malloc(len * sizeof(seq_t));
    if (!comb) return 0;        /* allocation failure → return 0 */

    comb[0] = 1;
    for (idx_t k = 1; k <= two_n; ++k) {
        comb[k] = comb[k - 1] * (seq_t)(two_n - (k - 1)) / (seq_t)k;
    }

    seq_t sum1 = 0, sum2 = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t t = k + 2;            /* step */
        for (idx_t i = 0; ; ++i) {
            /* First term: C(2n, n - i*t) */
            idx_t idx1 = n - i * t;
            if (i * t > n) break;  /* idx1 negative → further i give zero */
            sum1 += comb[idx1];

            /* Second term: C(2n, n + i*t + t - 1) */
            idx_t idx2 = n + i * t + t - 1;
            if (idx2 <= two_n) sum2 += comb[idx2];
        }
    }

    free(comb);
    return sum1 - sum2;
}
