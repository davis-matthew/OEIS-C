/*
 * A000279  Card matching: coefficients B[n,1] of t in the reduced hit
 *          polynomial A[n,n,n](t).
 * https://oeis.org/A000279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3n * Sum_{k=0..n-1} C(n,k+1)*C(n,k)*C(n-1,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k, m, j;

    if (seq_is_fixed_width()) {
        /* Pascal's triangle with additions only: exact modulo 2^w,
           so the result is exact whenever it fits. */
        seq_t *r = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        for (j = 0; j <= n; j++)
            r[j] = 0;
        r[0] = 1;
        for (m = 1; m <= n - 1; m++)
            for (j = m; j >= 1; j--)
                r[j] += r[j - 1];
        /* r[k] = C(n-1,k), r[n] = 0 */
        for (k = 0; k < n; k++) {
            seq_t cn1k = r[k];
            seq_t cnk = r[k] + (k ? r[k - 1] : (seq_t)0);
            seq_t cnk1 = r[k + 1] + r[k];
            s += cnk1 * cnk * cn1k;
        }
        free(r);
    } else {
        /* T(0) = n; T(k+1) = T(k)*(n-k-1)^2*(n-k) / ((k+2)*(k+1)^2) */
        seq_t t = (seq_t)n;
        s = t;
        for (k = 0; k + 1 < n; k++) {
            seq_t den = (seq_t)(k + 2) * (seq_t)(k + 1) * (seq_t)(k + 1);
            t = t * (seq_t)(n - k - 1) * (seq_t)(n - k - 1) * (seq_t)(n - k);
            t = t / den;
            s += t;
        }
    }
    return (seq_t)3 * (seq_t)n * s;
}
