/*
 * A007314  Reversion of o.g.f. for tangent numbers.
 * https://oeis.org/A007314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * [x^(n-1)] (x/F(x))^n (Lagrange inversion), where
 *            F(x) = Sum_{k>=1} T(k) x^k is the o.g.f. of the tangent numbers
 *            A000182 (1, 2, 16, 272, ...), computed by the Knuth-Buckholtz
 *            recurrence. Fixed-width builds use the division-free recurrence
 *            a(n) = -Sum_{k=2..n} T(k) [x^n] y^k, a(1) =...
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 15,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

typedef unsigned __int128 u128_t;

static seq_t small_path(idx_t n) {
    /* beyond this the value cannot fit in any fixed-width seq_t */
    if (n > 200) return 0;
    idx_t W = n + 1;
    u128_t *T = (u128_t *)calloc(W, sizeof(u128_t));
    u128_t *Q = (u128_t *)calloc(W * W, sizeof(u128_t));
    T[1] = 1;
    for (idx_t k = 2; k <= n; k++) T[k] = (u128_t)(k - 1) * T[k - 1];
    for (idx_t k = 2; k <= n; k++)
        for (idx_t j = k; j <= n; j++)
            T[j] = (u128_t)(j - k) * T[j - 1] + (u128_t)(j - k + 2) * T[j];
    /* Q[k*W + m] = [x^m] y^k */
    Q[1 * W + 1] = 1;
    for (idx_t m = 2; m <= n; m++) {
        for (idx_t k = 2; k <= m; k++) {
            u128_t s = 0;
            for (idx_t j = 1; j + k <= m + 1; j++)
                s += Q[1 * W + j] * Q[(k - 1) * W + (m - j)];
            Q[k * W + m] = s;
        }
        u128_t y = 0;
        for (idx_t k = 2; k <= m; k++) y -= T[k] * Q[k * W + m];
        Q[1 * W + m] = y;
    }
    u128_t r = Q[1 * W + n];
    free(T);
    free(Q);
    return u128_to_seq(r);
}

seq_t a(idx_t n) {
    if (!seq_is_big()) return small_path(n);
    if (n == 1) return 1;

    /* tangent numbers T[1..n] */
    seq_t *T = alloc_zero(n + 1);
    T[1] = 1;
    for (idx_t k = 2; k <= n; k++) T[k] = (seq_t)(k - 1) * T[k - 1];
    for (idx_t k = 2; k <= n; k++)
        for (idx_t j = k; j <= n; j++)
            T[j] = (seq_t)(j - k) * T[j - 1] + (seq_t)(j - k + 2) * T[j];

    /* g = x/F(x) = 1 / Sum_{i>=0} T[i+1] x^i, up to x^(n-1) */
    seq_t *g = alloc_zero(n);
    g[0] = 1;
    for (idx_t i = 1; i < n; i++) {
        seq_t s = 0;
        for (idx_t j = 1; j <= i; j++) s += T[j + 1] * g[i - j];
        g[i] = -s;
    }

    /* p = g^n by J.C.P. Miller's recurrence (g[0] = 1) */
    seq_t *p = alloc_zero(n);
    p[0] = 1;
    for (idx_t k = 1; k < n; k++) {
        seq_t s = 0;
        for (idx_t j = 1; j <= k; j++) {
            seq_t c = (seq_t)(n + 1) * (seq_t)j - (seq_t)k;
            s += c * g[j] * p[k - j];
        }
        p[k] = s / (seq_t)k;
    }

    seq_t result = p[n - 1] / (seq_t)n;
    release(T, n + 1);
    release(g, n);
    release(p, n);
    return result;
}
