/*
 * A002206  Numerators of logarithmic numbers (also of Gregory coefficients
 *          G(n)).
 * https://oeis.org/A002206
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, frac, nice
 * Formula:   a(n) = numerator(G(n+1)), where G(m) = (1/m!) * Sum_{k=0..m}
 *            Stirling1(m,k)/(k+1) (Gregory coefficients, 1/log(1+x) =
 *            Sum_{n>=-1} G(n+1) x^n). Computed exactly with multi-limb
 *            integers: T = Sum_k Stirling1(m,k) * (L/(k+1)) with L =
 *            lcm(1..m+1), so G(m) = T/(L*m!); T is then reduced by the
 *            primes...
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   g2206_add, g2206_cmp, g2206_divsmall, g2206_fma, g2206_modsmall,
 *            g2206_mul, g2206_spf, g2206_sub, g2206_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/g2206_add.h"
#include "../utils/g2206_cmp.h"
#include "../utils/g2206_divsmall.h"
#include "../utils/g2206_fma.h"
#include "../utils/g2206_modsmall.h"
#include "../utils/g2206_mul.h"
#include "../utils/g2206_spf.h"
#include "../utils/g2206_sub.h"
#include "../utils/g2206_trim.h"

seq_t a(idx_t n) {
    if (n < -1) return 0;
    size_t m = (size_t)(n + 1);
    size_t bl = 0, t = m + 1;
    while (t) { bl++; t >>= 1; }
    size_t cap = (m * bl) / 32 + 3;
    size_t Lcap = (2 * (m + 2)) / 32 + 3;
    size_t acap = cap + Lcap + 3;
    size_t j, k;

    uint32_t *S = (uint32_t *)calloc((m + 1) * cap, sizeof(uint32_t));
    size_t *len = (size_t *)calloc(m + 1, sizeof(size_t));
    S[0] = 1;
    len[0] = 1;
    /* unsigned Stirling numbers of the first kind: c(j+1,k) = c(j,k-1) + j*c(j,k) */
    for (j = 0; j < m; j++) {
        for (k = j + 1; k >= 1; k--)
            g2206_fma(S + k * cap, &len[k], S + (k - 1) * cap, len[k - 1], (uint32_t)j);
        len[0] = 0;
    }

    uint32_t *L = (uint32_t *)calloc(Lcap, sizeof(uint32_t));
    size_t Ll = 1;
    L[0] = 1;
    for (j = 2; j <= m + 1; j++) {
        size_t p = g2206_spf(j), q = j;
        while (q % p == 0) q /= p;
        if (q == 1) g2206_fma(L, &Ll, NULL, 0, (uint32_t)p);
    }

    uint32_t *Q = (uint32_t *)calloc(Lcap, sizeof(uint32_t));
    uint32_t *prod = (uint32_t *)calloc(acap, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(acap, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(acap, sizeof(uint32_t));
    size_t Pl = 0, Nl = 0;

    for (k = 0; k <= m; k++) {
        if (len[k] == 0) continue;
        size_t Ql = Ll;
        memcpy(Q, L, Ll * sizeof(uint32_t));
        g2206_divsmall(Q, &Ql, (uint32_t)(k + 1));
        size_t pl = g2206_mul(prod, S + k * cap, len[k], Q, Ql);
        if ((m - k) & 1) g2206_add(N, &Nl, prod, pl);
        else g2206_add(P, &Pl, prod, pl);
    }

    int neg = 0;
    uint32_t *T;
    size_t Tl;
    if (g2206_cmp(P, Pl, N, Nl) >= 0) {
        g2206_sub(P, &Pl, N, Nl);
        T = P; Tl = Pl;
    } else {
        g2206_sub(N, &Nl, P, Pl);
        T = N; Tl = Nl; neg = 1;
    }

    /* reduce by common factors with denominator L * m! */
    for (j = 2; j <= m + 1 && Tl > 0; j++) {
        if (g2206_spf(j) != j) continue;
        size_t p = j, e = 0, q;
        for (q = m; q > 0; q /= p) e += q / p;
        for (q = p; q <= m + 1; q *= p) e++;
        while (e > 0 && g2206_modsmall(T, Tl, (uint32_t)p) == 0) {
            g2206_divsmall(T, &Tl, (uint32_t)p);
            e--;
        }
    }

    seq_t v = 0;
    size_t i = Tl;
    while (i > 0) {
        i--;
        int b;
        for (b = 3; b >= 0; b--)
            v = v * 256 + (seq_t)((T[i] >> (8 * b)) & 255u);
    }
    if (neg) v = -v;

    free(S); free(len); free(L); free(Q); free(prod); free(P); free(N);
    return v;
}
