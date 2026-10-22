/*
 * A005191  Central pentanomial coefficients: largest coefficient of (1 + x +
 *          ... + x^4)^n.
 * https://oeis.org/A005191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(2n/5)} (-1)^k * binomial(n,k) *
 *            binomial(3n-5k-1, n-1) (coefficient of x^(2n) in
 *            (1-x^5)^n/(1-x)^n); a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    idx_t K, k, m, r, i, j, M;
    seq_t pos = 0, neg = 0;
    if (n == 0) return 1;
    K = 2 * n / 5;
    r = n - 1;
    if (seq_is_fixed_width()) {
        /* exact modulo 2^w via Pascal's triangle (additions only) */
        seq_t *row, *cn, *cm;
        M = 3 * n - 1;
        row = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        cn = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        cm = (seq_t *)malloc((size_t)(M + 1) * sizeof(seq_t));
        for (j = 0; j <= n; j++) { row[j] = 0; cn[j] = 0; }
        row[0] = 1;
        for (m = 0; m <= M; m++) {
            if (m >= 1) {
                idx_t top = m < n ? m : n;
                for (j = top; j >= 1; j--) row[j] += row[j - 1];
            }
            cm[m] = row[r];
            if (m == n)
                for (j = 0; j <= n; j++) cn[j] = row[j];
        }
        for (k = 0; k <= K; k++) {
            seq_t t = cn[k] * cm[M - 5 * k];
            if (k & 1) neg += t; else pos += t;
        }
        free(row); free(cn); free(cm);
    } else {
        seq_t cnk = 1, cb = 1;
        m = 3 * n - 1;
        for (i = 1; i <= r; i++)
            cb = cb * (seq_t)(m - r + i) / (seq_t)i;
        for (k = 0; k <= K; k++) {
            seq_t t = cnk * cb;
            if (k & 1) neg += t; else pos += t;
            if (k < K) {
                seq_t num = cb, den = 1;
                cnk = cnk * (seq_t)(n - k) / (seq_t)(k + 1);
                for (i = 0; i < 5; i++) {
                    num *= (seq_t)(m - r - i);
                    den *= (seq_t)(m - i);
                }
                cb = num / den;
                m -= 5;
            }
        }
    }
    return pos - neg;
}
