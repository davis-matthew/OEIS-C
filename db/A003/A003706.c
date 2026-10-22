/*
 * A003706  Expansion of e.g.f. sin(tan(x)), zeros omitted.
 * https://oeis.org/A003706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2n+1), where b(m) = d(m,0), d(0,x) = sin(x), d(m,x) =
 *            (1+x^2)*d/dx d(m-1,x). Each d(m,x) is kept as derivatives at 0:
 *            c'_j = c_{j+1} + j*(j-1)*c_{j-1}, starting from c_j = d^j/dx^j
 *            sin(x) at 0
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   from_i128, release, seq_is_fixed_width, zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/zalloc_s.h"
#include "../utils/release.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

typedef unsigned __int128 a003706_u128;

/* fixed-width builds: exact arithmetic mod 2^128 (no signed overflow) */
static seq_t a003706_fixed(idx_t N) {
    idx_t M = N + 2;
    a003706_u128 *c = (a003706_u128 *)calloc(M, sizeof(a003706_u128));
    a003706_u128 *d = (a003706_u128 *)calloc(M, sizeof(a003706_u128));
    a003706_u128 *t;
    idx_t i, j, k;
    seq_t r;
    for (i = 1; i < M; i += 2)
        c[i] = ((i / 2) % 2 == 0) ? (a003706_u128)1 : (a003706_u128)0 - 1;
    for (k = 0; k < N; k++) {
        idx_t lim = N - k - 1;
        for (j = (k & 1); j <= lim; j += 2) {
            a003706_u128 v = c[j + 1];
            if (j >= 1) v += c[j - 1] * ((a003706_u128)j * (a003706_u128)(j - 1));
            d[j] = v;
        }
        t = c; c = d; d = t;
    }
    r = from_i128((__int128)c[0]);
    free(c);
    free(d);
    return r;
}

static seq_t a003706_big(idx_t N) {
    idx_t M = N + 2;
    seq_t *c = zalloc_s(M);
    seq_t *d = zalloc_s(M);
    seq_t *t;
    idx_t i, j, k;
    seq_t r;
    for (i = 1; i < M; i += 2) {
        if ((i / 2) % 2 == 0) c[i] = 1;
        else c[i] = -1;
    }
    for (k = 0; k < N; k++) {
        idx_t lim = N - k - 1;
        for (j = (k & 1); j <= lim; j += 2) {
            seq_t v = c[j + 1];
            if (j >= 1) v += c[j - 1] * (seq_t)j * (seq_t)(j - 1);
            d[j] = v;
        }
        t = c; c = d; d = t;
    }
    r = c[0];
    release(c, M);
    release(d, M);
    return r;
}

seq_t a(idx_t n) {
    idx_t N = 2 * n + 1;
    if (seq_is_fixed_width()) return a003706_fixed(N);
    return a003706_big(N);
}
