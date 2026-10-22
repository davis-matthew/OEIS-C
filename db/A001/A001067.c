/*
 * A001067  Numerator of Bernoulli(2*n)/(2*n).
 * https://oeis.org/A001067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, nice
 * Formula:   a(n) = (-1)^(n-1) * numerator(T(n)/(4^n*(4^n-1))), since
 *            Bernoulli(2*n)/(2*n) = (-1)^(n-1)*T(n)/(4^n*(4^n-1)) with T(n)
 *            the tangent numbers (A000182). T(n) comes from the Brent-Harvey
 *            triangle recurrence. The numerator is
 *            T(n)*A006953(n)/(4^n*(4^n-1)), where by von Staudt-Clausen
 *            A006953(n) = Pro...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   add_limbs, bitlen_u, shr_limbs, small_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/add_limbs.h"
#include "../utils/bitlen_u.h"
#include "../utils/shr_limbs.h"
#include "../utils/small_isprime.h"

static uint32_t **tl = NULL;
static size_t *tlen = NULL;
static idx_t tN = 0;

static void build(idx_t N) {
    idx_t j, k;
    uint64_t bits = 0;
    if (tl) {
        for (j = 1; j <= tN; j++) free(tl[j]);
        free(tl);
        free(tlen);
    }
    tl = (uint32_t **)calloc(N + 1, sizeof(uint32_t *));
    tlen = (size_t *)calloc(N + 1, sizeof(size_t));
    for (j = 1; j <= N; j++) {
        bits += (uint64_t)bitlen_u(2 * j - 1) + (uint64_t)bitlen_u(2 * j);
        tl[j] = (uint32_t *)calloc(bits / 32 + 4, sizeof(uint32_t));
    }
    tl[1][0] = 1;
    tlen[1] = 1;
    for (j = 2; j <= N; j++) {
        uint64_t carry = 0, m = j - 1;
        size_t i, L = tlen[j - 1];
        for (i = 0; i < L; i++) {
            uint64_t v = m * tl[j - 1][i] + carry;
            tl[j][i] = (uint32_t)v;
            carry = v >> 32;
        }
        while (carry) { tl[j][L++] = (uint32_t)carry; carry >>= 32; }
        tlen[j] = L;
    }
    for (k = 2; k <= N; k++) {
        for (j = k; j <= N; j++) {
            uint64_t a = j - k, b = j - k + 2, carry = 0;
            const uint32_t *P = tl[j - 1];
            uint32_t *Q = tl[j];
            size_t lp = tlen[j - 1], lq = tlen[j];
            size_t L = lp > lq ? lp : lq, i;
            for (i = 0; i < L; i++) {
                uint64_t v = carry;
                if (a && i < lp) v += a * P[i];
                if (i < lq) v += b * Q[i];
                Q[i] = (uint32_t)v;
                carry = v >> 32;
            }
            while (carry) { Q[L++] = (uint32_t)carry; carry >>= 32; }
            tlen[j] = L;
        }
    }
    tN = N;
}

seq_t a(idx_t n) {
    uint64_t m = 2 * n, d, cnt = 0;
    size_t xcap, xl, yl, fcap, i;
    uint32_t *X, *Y, *F, *tmp;
    uint64_t y0, f0, M, inv, e;
    seq_t v = 0;
    int it;

    if (n > tN) {
        idx_t N = tN + tN / 2;
        if (N < n) N = n;
        if (N < 16) N = 16;
        build(N);
    }

    for (d = 1; d <= m; d++)
        if (m % d == 0 && small_isprime(d + 1)) {
            uint64_t p = d + 1, t = m;
            cnt++;
            while (t % p == 0) { cnt++; t /= p; }
        }

    xcap = tlen[n] + (size_t)cnt + 4;
    X = (uint32_t *)calloc(xcap, sizeof(uint32_t));
    memcpy(X, tl[n], tlen[n] * sizeof(uint32_t));
    xl = tlen[n];
    for (d = 1; d <= m; d++)
        if (m % d == 0 && small_isprime(d + 1)) {
            uint64_t p = d + 1, t = m, c = 1;
            while (t % p == 0) { c++; t /= p; }
            while (c--) {
                uint64_t carry = 0;
                for (i = 0; i < xl; i++) {
                    uint64_t w = p * X[i] + carry;
                    X[i] = (uint32_t)w;
                    carry = w >> 32;
                }
                while (carry) { X[xl++] = (uint32_t)carry; carry >>= 32; }
            }
        }

    Y = (uint32_t *)calloc(xl + 2, sizeof(uint32_t));
    yl = shr_limbs(Y, X, xl, m);

    fcap = yl + 3;
    F = (uint32_t *)calloc(fcap, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(yl + 2, sizeof(uint32_t));
    {
        uint64_t s;
        for (s = m; s < (uint64_t)yl * 32; s += m) {
            size_t tlg = shr_limbs(tmp, Y, yl, s);
            if (tlg == 0) break;
            add_limbs(F, fcap, tmp, tlg);
        }
    }

    y0 = (yl > 0 ? Y[0] : 0) | ((uint64_t)(yl > 1 ? Y[1] : 0) << 32);
    f0 = (uint64_t)F[0] | ((uint64_t)F[1] << 32);
    M = m >= 64 ? ~(uint64_t)0 : (((uint64_t)1 << m) - 1);
    inv = M;
    for (it = 0; it < 6; it++) inv *= 2 - M * inv;
    e = y0 * inv - f0;
    {
        uint32_t ev[2];
        ev[0] = (uint32_t)e;
        ev[1] = (uint32_t)(e >> 32);
        add_limbs(F, fcap, ev, 2);
    }

    {
        size_t fl = fcap;
        while (fl > 0 && F[fl - 1] == 0) fl--;
        for (i = fl; i-- > 0;) {
            v = v * 65536 + (seq_t)(int)(F[i] >> 16);
            v = v * 65536 + (seq_t)(int)(F[i] & 0xffff);
        }
    }

    free(X);
    free(Y);
    free(F);
    free(tmp);

    if (n % 2 == 0) v = -v;
    return v;
}
