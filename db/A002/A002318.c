/*
 * A002318  Expansion of (1/theta_4(q)^2 -1)/4 in powers of q.
 * https://oeis.org/A002318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   1/theta_4(q) = Sum pbar(m) q^m (overpartitions), pbar(m) =
 *            2*Sum_{k>=1} (-1)^(k+1) pbar(m-k^2), pbar(0) = 1; with h(m) =
 *            pbar(m)/2 for m >= 1: a(n) = h(n) + Sum_{i=1..n-1} h(i)*h(n-i)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 48, u64 n <= 151, u128 n <= 511,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t *h = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    idx_t m, k, i;
    seq_t r;

    for (m = 1; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1; k * k <= m; k++) {
            idx_t j = m - k * k;
            seq_t v;
            if (j == 0) v = 1;
            else v = h[j] * 2;
            if (k & 1) pos += v;
            else neg += v;
        }
        h[m] = pos - neg;
    }

    r = h[n];
    for (i = 1; i < n; i++)
        r += h[i] * h[n - i];

    free(h);
    return r;
}
