/*
 * A000631  Number of ethylene derivatives with n carbon atoms.
 * https://oeis.org/A000631
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{i=1..n-1} t(i)*t(n-i) + [n even]*t(n/2)) / 2, where
 *            t(k) = (Sum_{i=0..k-1} r(i)*r(k-1-i) + [k odd]*r((k-1)/2)) / 2
 *            (rooted trees, root out-degree <= 2, other nodes <= 3), and
 *            r(m+1) = ([x^m] R^3 + 3*[x^m] R(x)R(x^2) + 2*[x^m] R(x^3)) / 6,
 *            r(0) = 1 (A000598, rooted ternary trees). G.f...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 49, u128 n <= 92,
 *            big any n
 * Helpers:   alloc_zero, nofree_seq, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/nofree_seq.h"

typedef unsigned __int128 u128_t;

static u128_t *zalloc128(idx_t len) {
    return (u128_t *)calloc((size_t)len, sizeof(u128_t));
}
static void free128(u128_t *p) { free(p); }

#define GEN(T, S, ALLOC, FREE)                                              \
static T *r_##S = 0, *t_##S = 0, *s2_##S = 0, *s3_##S = 0;                  \
static idx_t cap_##S = 0, done_##S = 0;                                     \
static T *grow_##S(T *old, idx_t oldlen, idx_t newlen) {                    \
    T *p = ALLOC(newlen);                                                   \
    idx_t i;                                                                \
    for (i = 0; i < newlen; i++) {                                          \
        if (i < oldlen) p[i] = old[i]; else p[i] = 0;                       \
    }                                                                       \
    if (old) FREE(old);                                                     \
    return p;                                                               \
}                                                                           \
static void build_##S(idx_t n) {                                            \
    idx_t m, i;                                                             \
    if (n <= done_##S) return;                                              \
    if (n + 1 > cap_##S) {                                                  \
        idx_t nc = cap_##S + cap_##S / 2;                                   \
        if (nc < n + 1) nc = n + 1;                                         \
        if (nc < 64) nc = 64;                                               \
        r_##S = grow_##S(r_##S, cap_##S, nc);                               \
        t_##S = grow_##S(t_##S, cap_##S, nc);                               \
        s2_##S = grow_##S(s2_##S, cap_##S, nc);                             \
        s3_##S = grow_##S(s3_##S, cap_##S, nc);                             \
        if (cap_##S == 0) r_##S[0] = 1;                                     \
        cap_##S = nc;                                                       \
    }                                                                       \
    {                                                                       \
        T *r = r_##S, *t = t_##S, *s2 = s2_##S, *s3 = s3_##S;               \
        for (m = done_##S; m < n; m++) {                                    \
            T acc = 0, p = 0;                                               \
            for (i = 0; i <= m; i++) acc += r[i] * r[m - i];                \
            s2[m] = acc;                                                    \
            acc = 0;                                                        \
            for (i = 0; i <= m; i++) acc += s2[i] * r[m - i];               \
            s3[m] = acc;                                                    \
            for (i = 0; 2 * i <= m; i++) p += r[i] * r[m - 2 * i];          \
            acc = s3[m] + 3 * p;                                            \
            if (m % 3 == 0) acc += 2 * r[m / 3];                            \
            r[m + 1] = acc / 6;                                             \
            acc = s2[m];                                                    \
            if (m % 2 == 0) acc += r[m / 2];                                \
            t[m + 1] = acc / 2;                                             \
        }                                                                   \
    }                                                                       \
    done_##S = n;                                                           \
}                                                                           \
static T a_##S(idx_t n) {                                                   \
    T acc = 0;                                                              \
    idx_t i;                                                                \
    build_##S(n);                                                           \
    for (i = 1; i < n; i++) acc += t_##S[i] * t_##S[n - i];                 \
    if (n % 2 == 0) acc += t_##S[n / 2];                                    \
    return acc / 2;                                                         \
}

GEN(seq_t, big, alloc_zero, nofree_seq)
GEN(u128_t, fix, zalloc128, free128)

static int seq_fixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x *= 2;
    return x == 0;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    if (seq_fixed()) return u128_to_seq(a_fix(n));
    return a_big(n);
}
