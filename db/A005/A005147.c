/*
 * A005147  Denominators of numbers occurring in continued fraction connected
 *          with expansion of gamma function.
 * https://oeis.org/A005147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice
 * Formula:   Stieltjes continued fraction of the gamma function, J(z) =
 *            Sum_{k>=1} B_{2k}/(2k(2k-1) z^(2k-1)) =
 *            c(0)/(z+c(1)/(z+c(2)/(z+...))), a(n) = denominator(c(n)). With
 *            moments m(k) = |B_{2k+2}|/((2k+2)(2k+1)) =
 *            T(k+1)/(4^(k+1)(4^(k+1)-1)(2k+1)), T = tangent numbers, the qd
 *            algorithm gives q_1^(k) = m(k...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128w;

static u128w *u128w_alloc(idx_t n)
{
    idx_t i;
    u128w *p = (u128w *)malloc((size_t)(n + 1) * sizeof(u128w));
    for (i = 0; i <= n; i++) p[i] = 0;
    return p;
}

static void u128w_free(u128w *p, idx_t n)
{
    (void)n;
    free(p);
}

#define DEF_ALG(T, S, ALLOC, FREE) \
static T S##_gcd(T x, T y) \
{ \
    while (y != 0) { T t = x % y; x = y; y = t; } \
    return x; \
} \
static void S##_norm(T *p, T *q) \
{ \
    T g = S##_gcd(*p, *q); \
    if (g > 1) { *p /= g; *q /= g; } \
} \
static void S##_mul(T ap, T aq, T bp, T bq, T *rp, T *rq) \
{ \
    T g1 = S##_gcd(ap, bq), g2 = S##_gcd(bp, aq); \
    if (g1 == 0) g1 = 1; \
    if (g2 == 0) g2 = 1; \
    *rp = (ap / g1) * (bp / g2); \
    *rq = (aq / g2) * (bq / g1); \
} \
static void S##_add(T ap, T aq, T bp, T bq, T *rp, T *rq) \
{ \
    T g = S##_gcd(aq, bq); \
    T x = bq / g, y = aq / g; \
    *rp = ap * x + bp * y; \
    *rq = aq * x; \
    S##_norm(rp, rq); \
} \
static void S##_sub(T ap, T aq, T bp, T bq, T *rp, T *rq) \
{ \
    T g = S##_gcd(aq, bq); \
    T x = bq / g, y = aq / g; \
    *rp = ap * x - bp * y; \
    *rq = aq * x; \
    S##_norm(rp, rq); \
} \
static T S##_den(idx_t n) \
{ \
    idx_t N = n + 1, k, j, r, len; \
    T *t = ALLOC(N + 1); \
    T *qp = ALLOC(n), *qq = ALLOC(n), *ep = ALLOC(n), *eq = ALLOC(n); \
    T f1 = 4, res = 0, tp = 0, tq = 1; \
    t[1] = 1; \
    for (k = 2; k <= N; k++) t[k] = t[k - 1] * (k - 1); \
    for (k = 2; k <= N; k++) \
        for (j = k; j <= N; j++) \
            t[j] = t[j - 1] * (j - k) + t[j] * (j - k + 2); \
    for (k = 0; k < n; k++) { \
        T f2 = f1 * 4; \
        qp[k] = t[k + 2] * (f1 - 1) * (2 * k + 1); \
        qq[k] = t[k + 1] * 4 * (f2 - 1) * (2 * k + 3); \
        S##_norm(&qp[k], &qq[k]); \
        ep[k] = 0; \
        eq[k] = 1; \
        f1 = f2; \
    } \
    if (n == 1) { \
        res = qq[0]; \
    } else { \
        len = n; \
        for (r = 1;; r++) { \
            for (k = 0; k + 1 < len; k++) { \
                S##_add(qp[k + 1], qq[k + 1], ep[k + 1], eq[k + 1], &tp, &tq); \
                S##_sub(tp, tq, qp[k], qq[k], &ep[k], &eq[k]); \
            } \
            if (2 * r == n) { res = eq[0]; break; } \
            for (k = 0; k + 2 < len; k++) { \
                S##_mul(qp[k + 1], qq[k + 1], ep[k + 1], eq[k + 1], &tp, &tq); \
                S##_mul(tp, tq, eq[k], ep[k], &qp[k], &qq[k]); \
            } \
            len -= 2; \
            if (2 * r + 1 == n) { res = qq[0]; break; } \
        } \
    } \
    FREE(t, N + 1); \
    FREE(qp, n); \
    FREE(qq, n); \
    FREE(ep, n); \
    FREE(eq, n); \
    return res; \
}

DEF_ALG(seq_t, sq, alloc_zero, release)
DEF_ALG(u128w, wd, u128w_alloc, u128w_free)

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)12;
    if (seq_is_big()) return sq_den(n);
    return u128_to_seq(wd_den(n));
}
