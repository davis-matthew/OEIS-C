/*
 * A002687  Numerators of coefficients for repeated integration.
 * https://oeis.org/A002687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator((1/n!) * Sum_{k=1..n}
 *            Stirling1(n,k)/((k+1)*(k+2))), computed exactly with D =
 *            lcm(1..n+2): N = Sum Stirling1(n,k)*D/((k+1)(k+2)), value =
 *            N/(n!*D), reduced by removing the small primes of n!*D from N
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   z2687_add, z2687_bitlen, z2687_cmp, z2687_divs, z2687_isprime,
 *            z2687_len, z2687_mods, z2687_muladd, z2687_muls, z2687_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z2687_add.h"
#include "../utils/z2687_bitlen.h"
#include "../utils/z2687_cmp.h"
#include "../utils/z2687_divs.h"
#include "../utils/z2687_isprime.h"
#include "../utils/z2687_len.h"
#include "../utils/z2687_mods.h"
#include "../utils/z2687_muladd.h"
#include "../utils/z2687_muls.h"
#include "../utils/z2687_sub.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 2;
    uint64_t fb = 0;
    for (uint64_t i = 2; i <= n; i++) fb += (uint64_t)z2687_bitlen(i);
    size_t L = (size_t)((fb + 2 * m + 64) / 32 + 2);

    uint32_t *c = (uint32_t *)calloc((size_t)(n + 1) * L, sizeof(uint32_t));
    uint32_t *D = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *w = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *M = (uint32_t *)calloc(L, sizeof(uint32_t));
    if (!c || !D || !w || !P || !M) {
        free(c); free(D); free(w); free(P); free(M);
        return 0;
    }

    /* unsigned Stirling numbers of the first kind, row n */
    c[0] = 1;
    uint64_t rb = 0;
    for (uint64_t r = 1; r <= n; r++) {
        rb += (uint64_t)z2687_bitlen(r);
        size_t lim = (size_t)(rb / 32 + 2);
        if (lim > L) lim = L;
        for (uint64_t k = r; k >= 1; k--) {
            uint32_t *X = c + (size_t)k * L;
            z2687_muls(X, lim, (uint32_t)(r - 1));
            z2687_add(X, c + (size_t)(k - 1) * L, lim);
        }
        memset(c, 0, L * sizeof(uint32_t));
    }

    /* D = lcm(1..n+2) */
    D[0] = 1;
    for (uint64_t p = 2; p <= m; p++) {
        if (!z2687_isprime(p)) continue;
        uint64_t q = p;
        z2687_muls(D, L, (uint32_t)p);
        while (q <= m / p) { q *= p; z2687_muls(D, L, (uint32_t)p); }
    }

    for (uint64_t k = 1; k <= n; k++) {
        memcpy(w, D, L * sizeof(uint32_t));
        z2687_divs(w, L, (k + 1) * (k + 2));
        z2687_muladd(((n - k) & 1) ? M : P, c + (size_t)k * L, w, L);
    }

    int neg = 0;
    uint32_t *N;
    if (z2687_cmp(P, M, L) < 0) { z2687_sub(M, P, L); N = M; neg = 1; }
    else { z2687_sub(P, M, L); N = P; }

    /* cancel common factors with the denominator n! * D */
    for (uint64_t p = 2; p <= m; p++) {
        if (!z2687_isprime(p)) continue;
        uint64_t e = 0;
        for (uint64_t t = n / p; t > 0; t /= p) e += t;
        for (uint64_t q = p; q <= m; ) { e++; if (q > m / p) break; q *= p; }
        while (e > 0 && z2687_mods(N, L, p) == 0) {
            z2687_divs(N, L, p);
            e--;
        }
    }

    size_t ln = z2687_len(N, L);
    seq_t result = 0;
    for (size_t i = ln; i-- > 0;) {
        result = result * 65536 + (seq_t)(idx_t)(N[i] >> 16);
        result = result * 65536 + (seq_t)(idx_t)(N[i] & 0xFFFFu);
    }
    if (neg) result = -result;

    free(c); free(D); free(w); free(P); free(M);
    return result;
}
