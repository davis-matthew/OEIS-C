/*
 * A000957  Fine's sequence (or Fine numbers): number of relations of valence
 *          >= 1 on an n-set; also number of ordered rooted trees with n nodes
 *          having root of even degree.
 * https://oeis.org/A000957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (-1)^(n-1) * Sum_{k=0..n-1} (-1)^k * T(n-1,k), n >= 1,
 *            a(0) = 0, where T(m,k) = binomial(m+k,m)*(m-k+1)/(m+1) is the
 *            Catalan triangle A009766 (T(m,0) = 1, T(m,k) = T(m-1,k) +
 *            T(m,k-1), T(m,m) = T(m,m-1)). For arbitrary precision: a(n+1) =
 *            (Catalan(n) - a(n))/2, a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   a000957_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a000957_fixed.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (!a000957_fixed()) {
        seq_t av = 1, c = 1;
        idx_t k;
        for (k = 1; k < n; k++) {
            av = (c - av) / 2;
            c = c * (seq_t)(4 * k + 2) / (seq_t)(k + 2);
        }
        return av;
    }
    {
        idx_t m = n - 1, i, k;
        seq_t *r = (seq_t *)malloc((size_t)(m + 1) * sizeof(seq_t));
        seq_t pos = 0, neg = 0;
        r[0] = 1;
        for (i = 1; i <= m; i++) {
            for (k = 1; k < i; k++) r[k] += r[k - 1];
            r[i] = r[i - 1];
        }
        for (k = 0; k <= m; k++) {
            if (((m - k) & 1) == 0) pos += r[k];
            else neg += r[k];
        }
        free(r);
        return pos - neg;
    }
}
