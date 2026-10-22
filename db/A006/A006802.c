/*
 * A006802  Log of e.g.f. for trees A000055(n-1).
 * https://oeis.org/A006802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   E.g.f.: log(1 + Sum_{m>=1} A000055(m-1) x^m/m!). With f(0)=1,
 *            f(m)=A000055(m-1): a(1) = 1, a(m) = f(m) - Sum_{k=1..m-1}
 *            binomial(m-1,k-1)*a(k)*f(m-k). A000055(0)=1, A000055(n) = r(n) -
 *            (Sum_{k=1..n-1} r(k)*r(n-k) - [n even]*r(n/2))/2, where r =
 *            A000081: r(1)=1, r(n+1) = (1/n)*Sum_{k=1..n} (Sum_{d...
 * Range:     i8 n <= 9, i16 n <= 12, i32 n <= 17, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"

#define A6802_DEFINE_SOLVER(NAME, T, ALLOC, FREE)                          \
static void NAME(idx_t N, T *g) {                                          \
    T *r = ALLOC(N + 2);                                                   \
    T *s = ALLOC(N + 2);                                                   \
    T *f = ALLOC(N + 2);                                                   \
    T *c = ALLOC(N + 2);                                                   \
    idx_t n, k, d, j, m;                                                   \
    r[1] = 1;                                                              \
    for (n = 1; n <= N; n++) {                                             \
        for (d = n; d <= N; d += n) s[d] += (T)n * r[n];                   \
        if (n == N) break;                                                 \
        T acc = 0;                                                         \
        for (k = 1; k <= n; k++) acc += s[k] * r[n - k + 1];               \
        r[n + 1] = acc / (T)n;                                             \
    }                                                                      \
    f[0] = 1;                                                              \
    if (N >= 1) f[1] = 1;                                                  \
    for (m = 2; m <= N; m++) {                                             \
        idx_t t = m - 1;                                                   \
        T acc = 0;                                                         \
        for (k = 1; k < t; k++) acc += r[k] * r[t - k];                    \
        if (t % 2 == 0) acc -= r[t / 2];                                   \
        f[m] = r[t] - acc / (T)2;                                          \
    }                                                                      \
    c[0] = 1;                                                              \
    if (N >= 1) g[1] = f[1];                                               \
    for (m = 2; m <= N; m++) {                                             \
        for (j = m - 1; j >= 1; j--) c[j] += c[j - 1];                     \
        T acc = 0;                                                         \
        for (k = 1; k < m; k++) acc += c[k - 1] * g[k] * f[m - k];         \
        g[m] = f[m] - acc;                                                 \
    }                                                                      \
    FREE(r, N + 2);                                                        \
    FREE(s, N + 2);                                                        \
    FREE(f, N + 2);                                                        \
    FREE(c, N + 2);                                                        \
}

#define A6802_U_ALLOC(len) ((unsigned __int128 *)calloc((size_t)(len), sizeof(unsigned __int128)))
#define A6802_U_FREE(p, len) free(p)
#define A6802_S_ALLOC(len) alloc_zero(len)
#define A6802_S_FREE(p, len) release((p), (len))

/* Fixed-width builds: wrapping 128-bit arithmetic, exact whenever the term fits. */
A6802_DEFINE_SOLVER(a6802_solve_u, unsigned __int128, A6802_U_ALLOC, A6802_U_FREE)
/* Arbitrary-precision builds. */
A6802_DEFINE_SOLVER(a6802_solve_s, seq_t, A6802_S_ALLOC, A6802_S_FREE)

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    if (n < 1) return 0;
    if (n > cap) {
        idx_t N = cap * 2;
        if (N < n) N = n;
        if (N < 32) N = 32;
        seq_t *nc = alloc_zero(N + 1);
        if (seq_is_fixed_width()) {
            unsigned __int128 *gu = A6802_U_ALLOC(N + 1);
            idx_t i;
            a6802_solve_u(N, gu);
            for (i = 1; i <= N; i++) nc[i] = from_i128((__int128)gu[i]);
            free(gu);
        } else {
            a6802_solve_s(N, nc);
        }
        if (cache) release(cache, cap + 1);
        cache = nc;
        cap = N;
    }
    return cache[n];
}
