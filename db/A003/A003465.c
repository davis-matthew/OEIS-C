/*
 * A003465  Number of ways to cover an n-set.
 * https://oeis.org/A003465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n, k) * 2^(2^(n-k)) / 2,
 *            using 2^(2^m)/2 = p(m) with p(0) = 1, p(m+1) = 2*p(m)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t k, j;
    seq_t *row = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *p = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t pos = 0, neg = 0, result;

    /* binomial row n by Pascal's rule (exact, additions only) */
    for (k = 0; k <= n; ++k) row[k] = 0;
    row[0] = 1;
    for (j = 1; j <= n; ++j) {
        for (k = j; k >= 1; --k) row[k] = row[k] + row[k - 1];
    }

    /* p[m] = 2^(2^m - 1) */
    p[0] = 1;
    for (j = 1; j <= n; ++j) p[j] = p[j - 1] * p[j - 1] * 2;

    for (k = 0; k <= n; ++k) {
        seq_t term = row[k] * p[n - k];
        if (k % 2 == 0) pos = pos + term;
        else neg = neg + term;
    }
    result = pos - neg;

    free(row);
    free(p);
    return result;
}
