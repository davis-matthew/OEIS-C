/*
 * A007313  Reversion of g.f. for Euler (secant) numbers A000364.
 * https://oeis.org/A007313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   A(x) = Sum_{n>=1} a(n) x^n is the series reversion of G(x) =
 *            Sum_{j>=1} A000364(j-1) x^j = x + x^2 + 5x^3 + 61x^4 + ..., i.e.
 *            G(A(x)) = x. Equivalently a(1) = 1 and, for k >= 2, a(k) =
 *            -[x^k] Sum_{j=2..k} A000364(j-1) * A(x)^j. A000364 comes from
 *            the Seidel boustrophedon (A000364(m) = A000111(2m))
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *F = NULL;
static idx_t FN = 0;

static void build(idx_t N) {
    idx_t W = 2 * N + 2;
    seq_t *prev = alloc_zero(W);
    seq_t *cur = alloc_zero(W);
    seq_t *E = alloc_zero(N + 1);
    seq_t *f = alloc_zero(N + 1);
    seq_t *pw = alloc_zero((N + 1) * (N + 1));
    idx_t R = N + 1;

    /* Euler (secant) numbers via the boustrophedon transform */
    prev[0] = 1;
    E[0] = 1;
    for (idx_t k = 1; k + 2 <= 2 * N; k++) {
        cur[0] = 0;
        for (idx_t j = 1; j <= k; j++) cur[j] = cur[j - 1] + prev[k - j];
        seq_t *t = prev; prev = cur; cur = t;
        if (k % 2 == 0 && k / 2 <= N) E[k / 2] = prev[k];
    }

    /* reversion: pw[j*R+k] = [x^k] f^j */
    f[1] = 1;
    pw[0 * R + 0] = 1;
    pw[1 * R + 1] = 1;
    for (idx_t k = 2; k <= N; k++) {
        seq_t s = 0;
        for (idx_t j = 2; j <= k; j++) {
            seq_t c = 0;
            for (idx_t i = 1; i + j - 1 <= k; i++)
                c += f[i] * pw[(j - 1) * R + (k - i)];
            pw[j * R + k] = c;
            s += E[j - 1] * c;
        }
        f[k] = -s;
        pw[1 * R + k] = f[k];
    }

    release(prev, W);
    release(cur, W);
    release(E, N + 1);
    release(pw, (N + 1) * (N + 1));
    if (F) release(F, FN + 1);
    F = f;
    FN = N;
}

seq_t a(idx_t n) {
    if (n > FN) {
        idx_t N = n;
        if (N < 2 * FN) N = 2 * FN;
        if (N < 16) N = 16;
        build(N);
    }
    return F[n];
}
