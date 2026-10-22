/*
 * A002609  Glaisher's function G(n) (18 squares version).
 * https://oeis.org/A002609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   G.f.: theta(q)^6 * F(q) * (theta(q)^4 - 16*F(q)) * (theta(q)^4 -
 *            32*F(q)), where theta(q) = Sum_{k in Z} q^(k^2) and F(q) =
 *            Sum_{m odd} sigma(m) q^m. (theta^4-16F)(theta^4-32F) = W(q) with
 *            W_0 = 1 and, for k >= 1, W_k = -32*sigma_3(k) + 528*sigma_3(k/2)
 *            - 256*sigma_3(k/4). So a(n) = Sum_{i=1..n}...
 * Range:     i8 n <= 3, i16 n <= 14, i32 n <= 187, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_z.h"

static seq_t *Vc = NULL;
static seq_t *Wc = NULL;
static idx_t capc = 0;

static void build(idx_t N) {
    seq_t *V = a001028_z(N + 1);
    seq_t *T = a001028_z(N + 1);
    seq_t *W = a001028_z(N + 1);
    idx_t d, m, k, j;
    int pass;

    /* F(q): coefficient sigma(m) at odd m */
    for (d = 1; d <= N; d += 2)
        for (m = d; m <= N; m += 2 * d)
            V[m] += d;

    /* multiply by theta six times */
    for (pass = 0; pass < 6; pass++) {
        seq_t *tp;
        for (k = 0; k <= N; k++) {
            T[k] = V[k];
            for (j = 1; j * j <= k; j++)
                T[k] += V[k - j * j] * 2;
        }
        tp = V; V = T; T = tp;
    }

    /* sigma_3 sieve into W, then transform in place (descending) */
    for (d = 1; d <= N; d++) {
        seq_t c = (seq_t)d;
        c = c * (seq_t)d;
        c = c * (seq_t)d;
        for (m = d; m <= N; m += d)
            W[m] += c;
    }
    for (k = N; k >= 1; k--) {
        seq_t w = 0;
        w -= W[k] * 32;
        if (k % 2 == 0) w += W[k / 2] * 528;
        if (k % 4 == 0) w -= W[k / 4] * 256;
        W[k] = w;
    }
    W[0] = 1;

    Vc = V;
    Wc = W;
    capc = N;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t i;
    if (n > capc) {
        idx_t N = capc * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        build(N);
    }
    for (i = 1; i <= n; i++)
        r += Vc[i] * Wc[n - i];
    return r;
}
