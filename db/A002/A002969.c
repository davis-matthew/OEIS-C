/*
 * A002969  E.g.f. 1/(1 - sin(x) + sin(x)^2).
 * https://oeis.org/A002969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   E.g.f. A(x) = 1/f(x) with f(x) = 1 - sin(x) + sin(x)^2, so a(0)
 *            = 1, a(n) = -Sum_{k=1..n} binomial(n,k)*f(k)*a(n-k), where f(k)
 *            = -(-1)^((k-1)/2) for odd k, and f(k) = -(-1)^(k/2)*2^(k-1) for
 *            even k >= 2
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 13, i64 n <= 19, i128 n <= 20,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A_grow.h"

static seq_t *a002969_memo = 0;
static idx_t a002969_len = 0;

seq_t a(idx_t n) {
    if (n < a002969_len) return a002969_memo[n];
    a002969_memo = A_grow(a002969_memo, a002969_len, n + 1);
    if (a002969_len == 0) {
        a002969_memo[0] = 1;
        a002969_len = 1;
    }
    for (idx_t m = a002969_len; m <= n; m++) {
        seq_t s = 0;
        seq_t b = 1;   /* binomial(m, k) */
        seq_t pw = 1;  /* 2^(k-1) */
        for (idx_t k = 1; k <= m; k++) {
            b = b * (seq_t)(m - k + 1) / (seq_t)k;
            if (k > 1) pw = pw * 2;
            seq_t term = b * a002969_memo[m - k];
            if (k & 1) {
                /* f(k) = -1 if (k-1)/2 even, +1 otherwise */
                if ((((k - 1) / 2) & 1) == 0) s -= term;
                else s += term;
            } else {
                /* f(k) = -2^(k-1) if k/2 even, +2^(k-1) otherwise */
                if (((k / 2) & 1) == 0) s -= pw * term;
                else s += pw * term;
            }
        }
        a002969_memo[m] = -s;
        a002969_len = m + 1;
    }
    return a002969_memo[n];
}
