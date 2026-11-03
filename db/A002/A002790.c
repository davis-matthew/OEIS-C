/*
 * A002790  Denominators of Cauchy numbers of second type (= Bernoulli numbers
 *          B_n^{(n)}).
 * https://oeis.org/A002790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = denominator of Integral_{x=0..1} x(x+1)...(x+n-1) dx =
 *            denominator of Sum_{k=0..n} |Stirling1(n,k)|/(k+1). For each
 *            prime p <= n+1, let E = floor(log_p(n+1)) and M = p^E. The code
 *            computes S = Sum_k c_k * p^E/(k+1) mod M, where c_k are the
 *            coefficients of the rising factorial mod M. The p-...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 71, u64 n <= 335,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 986 (2026-11-01)
 * Helpers:   modinv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/modinv.h"

/* exponent of p in the denominator when p^2 > n+1 (E = 1) */
static idx_t a2790_large(idx_t n, uint64_t p)
{
    uint64_t A = n / p, b = n % p;
    long long mmax = (long long)(b + A + 1) - (long long)p;
    uint64_t *e, S = 0, C = 1, i, j;
    long long m;

    if (mmax < 0) return 0;
    e = (uint64_t *)calloc((size_t)mmax + 1, sizeof(uint64_t));
    e[0] = 1;
    for (j = 0; j < b; j++) {
        uint64_t jj = j % p;
        long long top = (long long)(j + 1) < mmax ? (long long)(j + 1) : mmax;
        for (m = top; m >= 1; m--) e[m] = (e[m] + jj * e[m - 1]) % p;
    }
    for (i = 0; i <= A; i++) {
        uint64_t t = p - 1 - A + i, term;
        if (t > b) break;
        term = (C * e[b - t]) % p;
        if ((A - i) & 1) term = (p - term) % p;
        term = (term * modinv((i + 1) % p, p)) % p;
        S = (S + term) % p;
        C = (C * ((A - i) % p)) % p;
        C = (C * modinv((i + 1) % p, p)) % p;
    }
    free(e);
    return S != 0 ? 1 : 0;
}

/* exponent of p in the denominator, general case: O(n^2) mod M */
static idx_t a2790_small(idx_t n, uint64_t p, idx_t E, uint64_t M)
{
    uint32_t *c = (uint32_t *)calloc((size_t)n + 2, sizeof(uint32_t));
    uint64_t cm = UINT64_MAX / M + 1;
    int fast = (M < 65536);
    idx_t j, k, deg = 0, v;
    uint64_t S = 0;

    c[0] = 1;
    for (j = 0; j < n; j++) {
        uint64_t jj = j % M;
        for (k = deg + 1; k >= 1; k--) {
            uint64_t x = (uint64_t)c[k - 1] + jj * c[k];
            if (fast)
                c[k] = (uint32_t)(((unsigned __int128)(cm * x) * M) >> 64);
            else
                c[k] = (uint32_t)(x % M);
        }
        c[0] = (uint32_t)((jj * c[0]) % M);
        deg++;
    }
    for (k = 0; k <= n; k++) {
        uint64_t u = k + 1, w;
        idx_t vv = 0, i;
        if (c[k] == 0) continue;
        while (u % p == 0) { u /= p; vv++; }
        w = modinv(u % M, M);
        for (i = vv; i < E; i++) w = (w * p) % M;
        S = (S + (uint64_t)c[k] * w) % M;
    }
    free(c);
    if (S == 0) return 0;
    v = 0;
    while (S % p == 0) { S /= p; v++; }
    return E - v;
}

seq_t a(idx_t n)
{
    idx_t lim = n + 1;
    unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
    seq_t r = 1;
    idx_t p, q;

    memset(comp, 0, (size_t)lim + 1);
    for (p = 2; p <= lim; p++) {
        idx_t f;
        if (comp[p]) continue;
        for (q = p * p; q <= lim; q += p) comp[q] = 1;
        if (p * p > lim) {
            f = a2790_large(n, p);
        } else {
            idx_t s = 0, m = n, E = 0;
            uint64_t M = 1;
            while (m > 0) { s += m % p; m /= p; }
            if (s < p - 1) continue;
            while (M * p <= lim) { M *= p; E++; }
            f = a2790_small(n, p, E, M);
        }
        while (f-- > 0) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
