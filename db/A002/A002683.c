/*
 * A002683  Numerators of coefficients for repeated integration.
 * https://oeis.org/A002683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of -(n/2)*M(n) - (2n+2)*M(n+1), where M(n) =
 *            (2/(2n+1)!)*Integral_{t=0..1} t*Product_{k=1..n} (t^2-k^2) =
 *            (1/(2n+1)!)*Integral_{u=0..1} Product_{k=1..n} (u-k^2) du.
 *            Evaluated exactly with internal multi-precision integers: X =
 *            -(n(2n+2)(2n+3)*S(n) + 4(n+1)*S(n+1)), Y = 2*L*(2n+3)...
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   q2683_S, q2683_addmul, q2683_cmp, q2683_divmod, q2683_muladd,
 *            q2683_sub, q2683_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/q2683_S.h"
#include "../utils/q2683_addmul.h"
#include "../utils/q2683_cmp.h"
#include "../utils/q2683_divmod.h"
#include "../utils/q2683_muladd.h"
#include "../utils/q2683_sub.h"
#include "../utils/q2683_trim.h"

seq_t a(idx_t n) {
    idx_t m1 = n + 1, N2 = n + 2, top = 2 * n + 3, k, j, p;
    unsigned char *comp = (unsigned char *)calloc((size_t)top + 1, 1);
    size_t Lcap, Ll, W, cap, sumbits = 0, l1, l2, Xl, i;
    uint32_t *L, *poly, *q, *pos, *neg, *S1, *S2;
    size_t *plen;
    int s1, s2, sx;
    seq_t v = 0;

    for (k = 2; k * k <= top; k++)
        if (!comp[k])
            for (j = k * k; j <= top; j += k) comp[j] = 1;

    /* L = lcm(1..n+2) */
    Lcap = (size_t)(N2 / 16) + 4;
    L = (uint32_t *)calloc(Lcap, sizeof(uint32_t));
    L[0] = 1; Ll = 1;
    for (p = 2; p <= N2; p++) {
        idx_t pp;
        if (comp[p]) continue;
        for (pp = p;; pp *= p) {
            Ll = q2683_muladd(L, Ll, (uint32_t)p, NULL, 0);
            if (pp > N2 / p) break;
        }
    }

    /* coefficient magnitude bound: prod (k^2+1) */
    for (k = 1; k <= m1; k++) {
        uint64_t t = (uint64_t)k * k + 1;
        while (t) { sumbits++; t >>= 1; }
    }
    W = sumbits / 32 + 2;
    poly = (uint32_t *)calloc((size_t)(m1 + 1) * W, sizeof(uint32_t));
    plen = (size_t *)calloc((size_t)(m1 + 1), sizeof(size_t));
    poly[0] = 1; plen[0] = 1;

    cap = W + Ll + 3;
    q = (uint32_t *)calloc(Ll + 1, sizeof(uint32_t));
    pos = (uint32_t *)calloc(cap, sizeof(uint32_t));
    neg = (uint32_t *)calloc(cap, sizeof(uint32_t));
    S1 = (uint32_t *)calloc(cap + 8, sizeof(uint32_t));
    S2 = (uint32_t *)calloc(cap + 8, sizeof(uint32_t));

    for (k = 1; k <= n; k++) {
        uint32_t k2 = (uint32_t)(k * k);
        for (j = k; j >= 1; j--)
            plen[j] = q2683_muladd(poly + j * W, plen[j], k2, poly + (j - 1) * W, plen[j - 1]);
        plen[0] = q2683_muladd(poly, plen[0], k2, NULL, 0);
    }
    l1 = q2683_S(poly, plen, W, n, L, Ll, q, pos, neg, cap, &s1);
    memcpy(S1, pos, l1 * sizeof(uint32_t));
    {
        uint32_t k2 = (uint32_t)(m1 * m1);
        for (j = m1; j >= 1; j--)
            plen[j] = q2683_muladd(poly + j * W, plen[j], k2, poly + (j - 1) * W, plen[j - 1]);
        plen[0] = q2683_muladd(poly, plen[0], k2, NULL, 0);
    }
    l2 = q2683_S(poly, plen, W, m1, L, Ll, q, pos, neg, cap, &s2);
    memcpy(S2, pos, l2 * sizeof(uint32_t));

    /* A = n(2n+2)(2n+3) S(n), B = 4(n+1) S(n+1) */
    l1 = q2683_muladd(S1, l1, (uint32_t)n, NULL, 0);
    l1 = q2683_muladd(S1, l1, (uint32_t)(2 * n + 2), NULL, 0);
    l1 = q2683_muladd(S1, l1, (uint32_t)(2 * n + 3), NULL, 0);
    l2 = q2683_muladd(S2, l2, 4u, NULL, 0);
    l2 = q2683_muladd(S2, l2, (uint32_t)(n + 1), NULL, 0);

    /* X = -(A + B), stored in S1 */
    if (s1 == s2) {
        Xl = q2683_muladd(S1, l1, 1u, S2, l2);
        sx = s1;
    } else if (q2683_cmp(S1, l1, S2, l2) >= 0) {
        Xl = q2683_sub(S1, l1, S2, l2);
        sx = s1;
    } else {
        Xl = q2683_sub(S2, l2, S1, l1);
        memcpy(S1, S2, Xl * sizeof(uint32_t));
        sx = s2;
    }
    sx = !sx;

    /* divide out common factors with Y = 2 * L * (2n+3)! */
    for (p = 2; p <= top && Xl > 0; p++) {
        idx_t e = 0, t, pp;
        if (comp[p]) continue;
        for (t = top; t; ) { t /= p; e += t; }
        if (p == 2) e++;
        if (p <= N2)
            for (pp = p;; pp *= p) {
                e++;
                if (pp > N2 / p) break;
            }
        while (e > 0 && q2683_divmod(S1, Xl, (uint32_t)p, 0) == 0) {
            q2683_divmod(S1, Xl, (uint32_t)p, 1);
            Xl = q2683_trim(S1, Xl);
            e--;
        }
    }

    for (i = Xl; i-- > 0;) {
        v = v * 65536 + (seq_t)(long long)(S1[i] >> 16);
        v = v * 65536 + (seq_t)(long long)(S1[i] & 0xFFFFu);
    }
    if (sx) v = -v;

    free(comp); free(L); free(poly); free(plen);
    free(q); free(pos); free(neg); free(S1); free(S2);
    return v;
}
