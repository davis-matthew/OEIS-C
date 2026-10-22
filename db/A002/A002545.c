/*
 * A002545  Numerator of Sum_{i+j+k=n; i,j,k > 0} 1/(i*j*k).
 * https://oeis.org/A002545
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 3*(H(n-1)^2 - H2(n-1))/n, where H(m) =
 *            Sum_{i=1..m} 1/i and H2(m) = Sum_{i=1..m} 1/i^2 (equivalently
 *            6*|Stirling1(n,3)|/n!, from G.f. (-log(1-x))^3). Computed with L
 *            = lcm(1..n-1): N = 3*(A^2 - B), D = n*L^2, A = Sum L/i, B = Sum
 *            L^2/i^2, then N is divided by gcd(N, D), found...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   q2545_add, q2545_divsm, q2545_modsm, q2545_mul, q2545_mulsm,
 *            q2545_sub, q2545_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q2545_add.h"
#include "../utils/q2545_divsm.h"
#include "../utils/q2545_modsm.h"
#include "../utils/q2545_mul.h"
#include "../utils/q2545_mulsm.h"
#include "../utils/q2545_sub.h"
#include "../utils/q2545_trim.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n - 1;
    size_t W = (size_t)(m / 16) + 8;
    size_t C = 4 * W + 8;
    uint32_t *L = (uint32_t *)calloc(C, sizeof(uint32_t));
    uint32_t *L2 = (uint32_t *)calloc(C, sizeof(uint32_t));
    uint32_t *A = (uint32_t *)calloc(C, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(C, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(C, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(C, sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc((size_t)n + 1, 1);
    size_t ll, l2l, al = 0, bl = 0, tl, nl;
    uint64_t p, i, j;
    seq_t r = 0;

    for (i = 2; i * i <= n; i++)
        if (!comp[i])
            for (j = i * i; j <= n; j += i) comp[j] = 1;

    L[0] = 1;
    ll = 1;
    for (p = 2; p <= m; p++) {
        uint64_t pk;
        if (comp[p]) continue;
        pk = p;
        while (pk * p <= m) pk *= p;
        ll = q2545_mulsm(L, ll, (uint32_t)pk);
    }
    l2l = q2545_mul(L2, L, ll, L, ll);

    for (i = 1; i <= m; i++) {
        memcpy(T, L, ll * sizeof(uint32_t));
        q2545_divsm(T, ll, (uint32_t)i);
        tl = q2545_trim(T, ll);
        al = q2545_add(A, al, T, tl);

        memcpy(T, L2, l2l * sizeof(uint32_t));
        q2545_divsm(T, l2l, (uint32_t)i);
        q2545_divsm(T, l2l, (uint32_t)i);
        tl = q2545_trim(T, l2l);
        bl = q2545_add(B, bl, T, tl);
    }

    nl = q2545_mul(N, A, al, A, al);
    nl = q2545_sub(N, nl, B, bl);
    nl = q2545_mulsm(N, nl, 3);

    for (p = 2; p <= n; p++) {
        uint64_t e = 0, t = n, pk;
        if (comp[p]) continue;
        while (t % p == 0) { t /= p; e++; }
        if (p <= m) {
            pk = p;
            e += 2;
            while (pk * p <= m) { pk *= p; e += 2; }
        }
        while (e > 0 && nl > 0 && q2545_modsm(N, nl, (uint32_t)p) == 0) {
            q2545_divsm(N, nl, (uint32_t)p);
            nl = q2545_trim(N, nl);
            e--;
        }
    }

    for (i = nl; i-- > 0;) {
        r = (seq_t)(r * (seq_t)65536 * (seq_t)65536 + (seq_t)N[i]);
    }

    free(L); free(L2); free(A); free(B); free(T); free(N); free(comp);
    return r;
}
