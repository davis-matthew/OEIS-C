/*
 * A004300  Number of symmetric irreducible diagrams with 2n nodes.
 * https://oeis.org/A004300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Let D(x) = Sum_{n>=0} (2n-1)!! x^n, B(x) = Sum b(n) x^n with
 *            b(n) = b(n-1) + 2(n-1)*b(n-2), b(0) = b(1) = 1 (symmetric
 *            diagrams, A047974), E(x) = D(x^2), P(x) = 1 - 1/D(x), U(x) = 1 -
 *            1/B(x), T(x) = U(x) - P(x^2). Then A(x) = Sum_{n>=1} a(n) x^n is
 *            determined by T(x) = Sum_{k>=1} a(k) x^k E(x)^(k...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *S_tab = 0;
static idx_t S_cap = 0;

static void build(idx_t N)
{
    idx_t i, j, k, n;
    idx_t H = N / 2 + 2;
    seq_t *D = alloc_zero(N + 1);
    seq_t *B = alloc_zero(N + 2);
    seq_t *P = alloc_zero(N + 1);
    seq_t *U = alloc_zero(N + 1);
    seq_t *T = alloc_zero(N + 1);
    seq_t *S = alloc_zero(N + 1);
    seq_t *acc = alloc_zero(N + 1);
    seq_t *row = alloc_zero(H);
    seq_t *prv = alloc_zero(H);
    seq_t *tmp;
    bool fixed = seq_is_fixed_width();

    D[0] = 1;
    for (i = 1; i <= N; i++) D[i] = D[i - 1] * (seq_t)(2 * i - 1);
    B[0] = 1;
    B[1] = 1;
    for (i = 2; i <= N; i++) B[i] = B[i - 1] + (seq_t)(2 * (i - 1)) * B[i - 2];

    for (n = 1; n <= N; n++) {
        seq_t s = 0, u = 0;
        for (j = 1; j < n; j++) {
            s += P[j] * D[n - j];
            u += U[j] * B[n - j];
        }
        P[n] = D[n] - s;
        U[n] = B[n] - u;
    }
    for (n = 1; n <= N; n++) {
        T[n] = U[n];
        if (n % 2 == 0) T[n] -= P[n / 2];
    }

    /* prv holds coefficients c[m][j] = [x^(2j)] E^m, starting with m = 0 */
    prv[0] = 1;
    for (k = 1; k <= N; k++) {
        idx_t m = k - 1;
        idx_t L = (N - k) / 2;
        if (m >= 1) {
            row[0] = 1;
            for (j = 1; j <= L; j++) {
                if (fixed) {
                    seq_t s = 0;
                    for (i = 0; i <= j; i++) s += D[i] * prv[j - i];
                    row[j] = s;
                } else {
                    row[j] = row[j - 1] + prv[j]
                           + ((seq_t)(2 * (j - 1)) * row[j - 1]) / (seq_t)m;
                }
            }
            tmp = row; row = prv; prv = tmp;
        }
        S[k] = T[k] - acc[k];
        for (j = 1; j <= L; j++) acc[k + 2 * j] += S[k] * prv[j];
    }

    release(D, N + 1);
    release(B, N + 2);
    release(P, N + 1);
    release(U, N + 1);
    release(T, N + 1);
    release(acc, N + 1);
    release(row, H);
    release(prv, H);

    if (S_tab) release(S_tab, S_cap + 1);
    S_tab = S;
    S_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > S_cap) {
        idx_t N = 2 * S_cap;
        if (N < n) N = n;
        if (N < 32) N = 32;
        build(N);
    }
    return S_tab[n];
}
