/*
 * A005760  Coefficients of modular function G_2(tau).
 * https://oeis.org/A005760
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = [q^n] G_2, where with f =
 *            q*Prod_{k>=1}(1-q^k)^2*(1-q^(11k))^2 (= eta(q)^2*eta(q^11)^2),
 *            G_2 = (-E4(q) + 14641*E4(q^11) - 249744*f^2 + 2904*f*(E2(q) -
 *            11*E2(q^11))) / (14640*f^2), E4(q) = 1 + 240*Sum sigma_3(k) q^k,
 *            E2(q) = 1 - 24*Sum sigma(k) q^k. (G_2 is the Weierstrass
 *            x-function of X_0...
 * Range:     i8 n <= 18, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

/* s <- s / Prod_{k>=1} (1 - q^(st*k)), over seq_t */
static void div_eta(seq_t *s, long long L, long long st) {
    for (long long k = 0; k < L; k++) {
        seq_t acc = s[k];
        for (long long m = 1;; m++) {
            long long g1 = st * (m * (3 * m - 1) / 2);
            long long g2 = st * (m * (3 * m + 1) / 2);
            if (g1 > k) break;
            if (m & 1) acc += s[k - g1]; else acc -= s[k - g1];
            if (g2 <= k) {
                if (m & 1) acc += s[k - g2]; else acc -= s[k - g2];
            }
        }
        s[k] = acc;
    }
}

/* s <- s * Prod_{k>=1} (1 - q^(st*k)), native integers */
static void mul_eta(long long *s, long long L, long long st) {
    for (long long k = L - 1; k >= 0; k--) {
        long long acc = s[k];
        for (long long m = 1;; m++) {
            long long g1 = st * (m * (3 * m - 1) / 2);
            long long g2 = st * (m * (3 * m + 1) / 2);
            if (g1 > k) break;
            if (m & 1) acc -= s[k - g1]; else acc += s[k - g1];
            if (g2 <= k) {
                if (m & 1) acc -= s[k - g2]; else acc += s[k - g2];
            }
        }
        s[k] = acc;
    }
}

seq_t a(idx_t n) {
    static seq_t *X = NULL;
    static long long len = 0;
    if (n < -2) return 0;
    long long need = (long long)n + 3;
    if (need > len) {
        long long L = need + need / 2 + 16;
        long long *s1 = (long long *)calloc((size_t)L, sizeof(long long));
        long long *s3 = (long long *)calloc((size_t)L, sizeof(long long));
        long long *F2 = (long long *)calloc((size_t)L, sizeof(long long));
        long long *GP = (long long *)calloc((size_t)L, sizeof(long long));
        for (long long d = 1; d < L; d++)
            for (long long m = d; m < L; m += d) {
                s1[m] += d;
                s3[m] += d * d * d;
            }
        /* F2 = f^2 / q^2 */
        F2[0] = 1;
        for (int r = 0; r < 4; r++) mul_eta(F2, L, 1);
        for (int r = 0; r < 4; r++) mul_eta(F2, L, 11);
        /* GP = f*P / q, P = E2(q) - 11*E2(q^11) */
        GP[0] = -10;
        for (long long k = 1; k < L; k++) {
            GP[k] = -24 * s1[k];
            if (k % 11 == 0) GP[k] += 264 * s1[k / 11];
        }
        for (int r = 0; r < 2; r++) mul_eta(GP, L, 1);
        for (int r = 0; r < 2; r++) mul_eta(GP, L, 11);
        /* Y = numerator / q^2, then divide by f^2 / q^2 */
        seq_t *Y = alloc_zero((idx_t)L);
        for (long long k = 0; k < L; k++) {
            long long v = (k == 0) ? 14640 : 0;
            if (k >= 1) v -= 240 * s3[k];
            if (k >= 11 && k % 11 == 0) v += 14641LL * 240 * s3[k / 11];
            if (k >= 2) v -= 249744LL * F2[k - 2];
            if (k >= 1) v += 2904LL * GP[k - 1];
            Y[k] = (seq_t)(v / 14640);
        }
        for (int r = 0; r < 4; r++) div_eta(Y, L, 1);
        for (int r = 0; r < 4; r++) div_eta(Y, L, 11);
        free(s1);
        free(s3);
        free(F2);
        free(GP);
        if (X != NULL) release(X, (idx_t)len);
        X = Y;
        len = L;
    }
    return X[n + 2];
}
