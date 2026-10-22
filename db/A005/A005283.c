/*
 * A005283  Number of permutations of (1,...,n) having n-5 inversions (n>=5).
 * https://oeis.org/A005283
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-5} e(j)*binomial(2*n-6-j, n-1), where Sum_j
 *            e(j)*x^j = Product_{i>=1} (1-x^i) (Euler pentagonal:
 *            e(m(3m-1)/2) = e(m(3m+1)/2) = (-1)^m), i.e. [x^(n-5)]
 *            Product_{i=1..n} (1-x^i)/(1-x)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    if (n < 5) return 0;
    idx_t k = n - 5;

    int *e = (int *)calloc((size_t)k + 1, sizeof(int));
    e[0] = 1;
    for (idx_t m = 1;; m++) {
        idx_t p1 = m * (3 * m - 1) / 2;
        idx_t p2 = m * (3 * m + 1) / 2;
        int s = (m & 1) ? -1 : 1;
        if (p1 > k) break;
        e[p1] = s;
        if (p2 <= k) e[p2] = s;
    }

    seq_t result;
    if (seq_is_fixed_width()) {
        /* exact modulo 2^w: start from pentagonal series, apply 1/(1-x) n times */
        seq_t *c = (seq_t *)malloc(((size_t)k + 1) * sizeof(seq_t));
        for (idx_t j = 0; j <= k; j++) {
            c[j] = 0;
            if (e[j] > 0) c[j] += 1;
            else if (e[j] < 0) c[j] -= 1;
        }
        for (idx_t t = 0; t < n; t++)
            for (idx_t j = 1; j <= k; j++)
                c[j] += c[j - 1];
        result = c[k];
        free(c);
    } else {
        /* C = binomial(n-1+r, r), r = 0..k; coefficient e(k-r) */
        seq_t pos = 0, neg = 0;
        seq_t C = 1;
        for (idx_t r = 0; r <= k; r++) {
            if (r > 0) {
                C *= (seq_t)(n - 1 + r);
                C /= (seq_t)r;
            }
            int s = e[k - r];
            if (s > 0) pos += C;
            else if (s < 0) neg += C;
        }
        result = pos - neg;
    }
    free(e);
    return result;
}
