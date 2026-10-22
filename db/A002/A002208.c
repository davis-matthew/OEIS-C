/*
 * A002208  Numerators of coefficients for numerical integration.
 * https://oeis.org/A002208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy, nice
 * Formula:   a(n) = numerator(((-1)^n/n!)*Sum_{k=0..n}
 *            Stirling1(n+1,k+1)/(k+1)), a(0) = 1; computed exactly from the
 *            coefficients c_k = |Stirling1(n+1,k+1)| of Product_{j=1..n}
 *            (x+j): a(n)/A002209(n) = (1/n!)*Sum_{k=0..n} (-1)^k*c_k/(k+1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   g2208_addmul, g2208_bitlen, g2208_divsmall, g2208_modsmall,
 *            g2208_mulsmall, g2208_norm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/g2208_addmul.h"
#include "../utils/g2208_bitlen.h"
#include "../utils/g2208_divsmall.h"
#include "../utils/g2208_modsmall.h"
#include "../utils/g2208_mulsmall.h"
#include "../utils/g2208_norm.h"

seq_t a(idx_t n) {
    size_t N, bits, W, j, m, k, i, Llen, XS, len;
    uint32_t *c, *L, *T, *Qp, *Qn, *Fp, *Fn, *X, *Y;
    unsigned char *comp;
    seq_t r;

    if (n == 0) return 1;
    N = (size_t)n;

    bits = 0;
    for (j = 1; j <= N + 1; j++) bits += (size_t)g2208_bitlen(j);
    W = bits / 32 + 2;

    /* coefficients of Product_{j=1..m} (x+j), each bounded by (m+1)! */
    c = (uint32_t *)calloc((N + 1) * W, sizeof(uint32_t));
    c[0] = 1;
    {
        size_t bm = 1; /* bitlen(1) */
        for (m = 1; m <= N; m++) {
            size_t cur;
            bm += (size_t)g2208_bitlen(m + 1);
            cur = bm / 32 + 2;
            if (cur > W) cur = W;
            for (k = m; k >= 1; k--) {
                uint32_t *ck = c + k * W;
                const uint32_t *cp = c + (k - 1) * W;
                uint64_t carry = 0;
                for (i = 0; i < cur; i++) {
                    uint64_t t = (uint64_t)ck[i] * (uint32_t)m + cp[i] + carry;
                    ck[i] = (uint32_t)t;
                    carry = t >> 32;
                }
            }
            g2208_mulsmall(c, cur, (uint32_t)m);
        }
    }

    /* primes up to N+1 */
    comp = (unsigned char *)calloc(N + 2, 1);
    for (i = 2; i * i <= N + 1; i++)
        if (!comp[i])
            for (j = i * i; j <= N + 1; j += i) comp[j] = 1;

    /* L = lcm(1..N+1) */
    L = (uint32_t *)calloc(W, sizeof(uint32_t));
    L[0] = 1;
    for (i = 2; i <= N + 1; i++) {
        if (!comp[i]) {
            uint64_t pp = i;
            while (pp * i <= N + 1) pp *= i;
            g2208_mulsmall(L, W, (uint32_t)pp);
        }
    }
    Llen = g2208_norm(L, W);

    Qp = (uint32_t *)calloc(W + 1, sizeof(uint32_t));
    Qn = (uint32_t *)calloc(W + 1, sizeof(uint32_t));
    Fp = (uint32_t *)calloc(Llen + 2, sizeof(uint32_t));
    Fn = (uint32_t *)calloc(Llen + 2, sizeof(uint32_t));
    T = (uint32_t *)calloc(Llen + 1, sizeof(uint32_t));

    /* Sum (-1)^k c_k/(k+1) = Sum (-1)^k q_k + Sum (-1)^k r_k/(k+1) */
    for (k = 0; k <= N; k++) {
        uint32_t *ck = c + k * W;
        uint32_t rem = g2208_divsmall(ck, W, (uint32_t)(k + 1));
        g2208_addmul((k & 1) ? Qn : Qp, W + 1, ck, W, 1);
        if (rem) {
            memcpy(T, L, Llen * sizeof(uint32_t));
            g2208_divsmall(T, Llen, (uint32_t)(k + 1));
            g2208_addmul((k & 1) ? Fn : Fp, Llen + 2, T, Llen, rem);
        }
    }

    XS = W + Llen + 3;
    X = (uint32_t *)calloc(XS, sizeof(uint32_t));
    Y = (uint32_t *)calloc(XS, sizeof(uint32_t));
    for (i = 0; i < Llen; i++) {
        g2208_addmul(X + i, XS - i, Qp, W + 1, L[i]);
        g2208_addmul(Y + i, XS - i, Qn, W + 1, L[i]);
    }
    g2208_addmul(X, XS, Fp, Llen + 2, 1);
    g2208_addmul(Y, XS, Fn, Llen + 2, 1);

    /* Num = X - Y (positive), value = Num / (L * N!) */
    {
        int64_t borrow = 0;
        for (i = 0; i < XS; i++) {
            int64_t t = (int64_t)X[i] - (int64_t)Y[i] - borrow;
            if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
            X[i] = (uint32_t)t;
        }
    }
    len = g2208_norm(X, XS);

    /* reduce by primes of the denominator L * N! */
    for (i = 2; i <= N + 1; i++) {
        if (!comp[i]) {
            uint64_t e = 0, pp = i, q;
            while (pp <= N + 1) { e++; pp *= i; }
            q = N;
            while (q) { q /= i; e += q; }
            while (e > 0 && len > 0 && g2208_modsmall(X, len, (uint32_t)i) == 0) {
                g2208_divsmall(X, len, (uint32_t)i);
                len = g2208_norm(X, len);
                e--;
            }
        }
    }

    r = 0;
    for (i = len; i > 0; i--) {
        unsigned hi = (unsigned)(X[i - 1] >> 16);
        unsigned lo = (unsigned)(X[i - 1] & 0xFFFFu);
        r = r * 65536u + hi;
        r = r * 65536u + lo;
    }

    free(c); free(comp); free(L); free(Qp); free(Qn);
    free(Fp); free(Fn); free(T); free(X); free(Y);
    return r;
}
