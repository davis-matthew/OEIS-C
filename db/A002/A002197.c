/*
 * A002197  Numerators of coefficients for numerical integration.
 * https://oeis.org/A002197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(b(n)), where 1/x - 1/(sqrt(x)*arcsin(sqrt(x)))
 *            = Sum_{n>=0} b(n)*x^n. With F(x) = arcsin(sqrt(x))/sqrt(x) =
 *            Sum_{k>=0} c(k)*x^k, c(k) = binomial(2k,k)/(4^k*(2k+1)), and G =
 *            1/F: g(0) = 1, g(m) = -Sum_{k=1..m} c(k)*g(m-k), and b(n) =
 *            -g(n+1). The denominator of b(n) divides M = 2*...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   q2197_add, q2197_div, q2197_isprime, q2197_mod, q2197_mul,
 *            q2197_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q2197_add.h"
#include "../utils/q2197_div.h"
#include "../utils/q2197_isprime.h"
#include "../utils/q2197_mod.h"
#include "../utils/q2197_mul.h"
#include "../utils/q2197_pw.h"

seq_t a(idx_t n)
{
    uint64_t N = n, T = N + 1, PM = 4 * N + 8, F = 2 * N + 3;
    uint64_t q, i, k, m, bl = 0, capM, K, cnt;
    unsigned char *comp;
    uint64_t *eM;
    uint32_t *M, *H, *ps, *res, *vv, *c, *g;
    size_t lenM = 1, lenH = 0;
    uint64_t chunk = 1;
    seq_t x = 0;

    /* sieve primes up to PM and exponents of M */
    comp = (unsigned char *)calloc(PM + 1, 1);
    eM = (uint64_t *)calloc(PM + 1, sizeof(uint64_t));
    for (q = 2; q * q <= PM; q++)
        if (!comp[q])
            for (i = q * q; i <= PM; i += q) comp[i] = 1;
    while ((1ULL << bl) <= F) bl++;
    capM = (1 + 2 * T + F * bl + 2 * PM) / 32 + 8;
    M = (uint32_t *)calloc(capM, sizeof(uint32_t));
    M[0] = 1;
    for (q = 2; q <= PM; q++) {
        uint64_t e = 0, t, pw = 1;
        if (comp[q]) continue;
        for (t = F / q; t; t /= q) e += t;
        while (pw * q <= PM) { pw *= q; e++; }
        if (q == 2) e += 1 + 2 * T;
        eM[q] = e;
        for (t = 0; t < e; t++) {
            if (chunk * q > 0xFFFFFFFFULL) {
                lenM = q2197_mul(M, lenM, (uint32_t)chunk);
                chunk = 1;
            }
            chunk *= q;
        }
    }
    if (chunk > 1) lenM = q2197_mul(M, lenM, (uint32_t)chunk);

    /* primes in (2^29, 2^30) whose product exceeds M */
    K = (32 * (uint64_t)lenM) / 29 + 2;
    ps = (uint32_t *)malloc(K * sizeof(uint32_t));
    res = (uint32_t *)malloc(K * sizeof(uint32_t));
    vv = (uint32_t *)malloc(K * sizeof(uint32_t));
    cnt = 0;
    for (q = (1ULL << 30) - 1; cnt < K; q -= 2)
        if (q2197_isprime(q)) ps[cnt++] = (uint32_t)q;

    c = (uint32_t *)malloc((T + 1) * sizeof(uint32_t));
    g = (uint32_t *)malloc((T + 1) * sizeof(uint32_t));
    for (i = 0; i < K; i++) {
        uint64_t p = ps[i];
        uint64_t Mm = q2197_mod(M, lenM, (uint32_t)p);
        c[0] = 1;
        for (k = 1; k <= T; k++) {
            uint64_t num = (2 * k - 1) % p, den = (2 * k) % p * ((2 * k + 1) % p) % p;
            num = num * num % p;
            c[k] = (uint32_t)((uint64_t)c[k - 1] * num % p * q2197_pw(den, p - 2, p) % p);
        }
        g[0] = 1;
        for (m = 1; m <= T; m++) {
            uint64_t s = 0;
            int ct = 0;
            for (k = 1; k <= m; k++) {
                s += (uint64_t)c[k] * g[m - k];
                if (++ct == 8) { s %= p; ct = 0; }
            }
            s %= p;
            g[m] = (uint32_t)(s ? p - s : 0);
        }
        res[i] = (uint32_t)((p - g[T]) % p * Mm % p);
    }

    /* Garner reconstruction */
    for (i = 0; i < K; i++) {
        uint64_t p = ps[i], acc = 0, prod = 1, j;
        for (j = 0; j < i; j++) {
            acc = (acc + (uint64_t)vv[j] * prod) % p;
            prod = prod * ps[j] % p;
        }
        vv[i] = (uint32_t)(((uint64_t)res[i] + p - acc) % p * q2197_pw(prod, p - 2, p) % p);
    }
    H = (uint32_t *)calloc(K + 4, sizeof(uint32_t));
    for (i = K; i-- > 0;) {
        lenH = q2197_mul(H, lenH, ps[i]);
        lenH = q2197_add(H, lenH, vv[i]);
    }

    /* cancel common factors with M */
    for (q = 2; q <= PM; q++) {
        uint64_t e = eM[q];
        while (e > 0 && lenH > 0 && q2197_mod(H, lenH, (uint32_t)q) == 0) {
            lenH = q2197_div(H, lenH, (uint32_t)q);
            e--;
        }
    }

    for (i = lenH; i-- > 0;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            idx_t b = (idx_t)((H[i] >> sh) & 255u);
            x = x * 256 + (seq_t)b;
        }
    }

    free(comp); free(eM); free(M); free(ps); free(res); free(vv);
    free(c); free(g); free(H);
    return x;
}
