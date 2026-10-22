/*
 * A002766  Number of bipartite partitions.
 * https://oeis.org/A002766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   G.f.: x * Product_{k>=1} 1/(1-x^k) * ( 1/(1-x) + 2/(1-x)^2 +
 *            1/((1-x)*(1-x^2)*(1-x^3)) ), i.e. a(n) = Sum_{k=0..n-1}
 *            p(k)*q(n-1-k) with q(j) = 3 + 2*j + round((j+3)^2/12), p(k) =
 *            number of partitions of k (Euler's pentagonal recurrence, p(0) =
 *            1)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t k, j;
    seq_t *p = (seq_t *)calloc((size_t)(m + 1), sizeof(seq_t));
    seq_t res = 0;

    p[0] = 1;
    for (k = 1; k <= m; k++) {
        seq_t pos = 0, neg = 0;
        for (j = 1;; j++) {
            idx_t g1 = j * (3 * j - 1) / 2;
            idx_t g2 = j * (3 * j + 1) / 2;
            if (g1 > k) break;
            if (j & 1) pos += p[k - g1]; else neg += p[k - g1];
            if (g2 <= k) {
                if (j & 1) pos += p[k - g2]; else neg += p[k - g2];
            }
        }
        p[k] = pos - neg;
    }

    for (k = 0; k <= m; k++) {
        uint64_t jj = (uint64_t)(m - k);
        uint64_t q = 3 + 2 * jj + ((jj + 3) * (jj + 3) + 6) / 12;
        res += p[k] * (seq_t)q;
    }

    free(p);
    return res;
}
