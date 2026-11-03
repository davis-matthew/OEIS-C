/*
 * A002433  Theta series of unique 26-dimensional unimodular lattice T_26 with
 *          no roots (and minimal norm 3).
 * https://oeis.org/A002433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (f^26 - 52*f^18*g + 156*f^10*g^2), with f =
 *            theta_3(q) and g = q*Prod_{m>=1} (1-q^(2m-1))^8*(1-q^(4m))^8 =
 *            theta_2(q)^4*theta_4(q)^4/16. Using theta_2(q)^4 = 16*q*T(q^2)^4
 *            with T(x) = Sum_{k>=0} x^(k(k+1)/2) and theta_3(q)*theta_4(q) =
 *            theta_4(q^2)^2, this becomes a(n) = [q^n] theta_...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 61, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 973 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply in place by T = Sum_{k>=0} x^(k(k+1)/2) */
static void t26_mul_T(seq_t *a, idx_t len)
{
    idx_t m, k, t;
    for (m = len; m-- > 1;) {
        for (k = 1, t = 1; t <= m; k++, t += k) a[m] += a[m - t];
    }
}

/* multiply in place by theta_3 = 1 + 2*Sum_{s>=1} x^(s^2) */
static void t26_mul_theta(seq_t *a, idx_t len)
{
    idx_t m, s;
    for (m = len; m-- > 1;) {
        seq_t acc = 0;
        for (s = 1; s * s <= m; s++) acc += a[m - s * s];
        a[m] += acc + acc;
    }
}

static seq_t *t26_zero(idx_t len)
{
    idx_t i;
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *t26_A13 = 0, *t26_A14 = 0, *t26_R2 = 0;
static seq_t *t26_V1P = 0, *t26_V1N = 0, *t26_V2P = 0, *t26_V2N = 0;
static idx_t t26_cap = 0;
static int t26_built = 0;

static void t26_build(idx_t C)
{
    idx_t L = C + 1, M = C / 2 + 1, i, d, j, r;
    seq_t *s3, *r8, *tmp;

    if (t26_built) {
        mem_free(t26_A13); mem_free(t26_A14); mem_free(t26_R2);
        mem_free(t26_V1P); mem_free(t26_V1N); mem_free(t26_V2P); mem_free(t26_V2N);
    }

    s3 = t26_zero(L);
    for (d = 1; d <= C; d++) {
        seq_t c = (seq_t)d * (seq_t)d * (seq_t)d;
        for (j = d; j <= C; j += d) s3[j] += c;
    }
    r8 = t26_zero(L);
    r8[0] = 1;
    for (i = 1; i <= C; i++) {
        if (i & 1) r8[i] = s3[i] * 16;
        else {
            idx_t o = i;
            while (!(o & 1)) o >>= 1;
            r8[i] = (s3[i / 2] * 8 - s3[o]) * 16;
        }
    }

    t26_A13 = t26_zero(L);
    for (i = 0; i < L; i++) t26_A13[i] = r8[i];
    for (r = 0; r < 5; r++) t26_mul_theta(t26_A13, L);
    t26_A14 = t26_zero(L);
    for (i = 0; i < L; i++) t26_A14[i] = t26_A13[i];
    t26_mul_theta(t26_A14, L);

    t26_R2 = t26_zero(L);
    t26_R2[0] = 1;
    t26_mul_theta(t26_R2, L);
    t26_mul_theta(t26_R2, L);

    t26_V1P = t26_zero(M);
    t26_V1N = t26_zero(M);
    for (i = 0; i < M; i++) {
        if (i & 1) t26_V1N[i] = r8[i];
        else t26_V1P[i] = r8[i];
    }
    for (r = 0; r < 4; r++) {
        t26_mul_T(t26_V1P, M);
        t26_mul_T(t26_V1N, M);
    }

    tmp = t26_zero(M);
    for (i = 0; i < M; i++) tmp[i] = t26_A14[i];
    t26_mul_theta(tmp, M);
    t26_mul_theta(tmp, M); /* theta_3^16 */
    t26_V2P = t26_zero(M);
    t26_V2N = t26_zero(M);
    for (i = 0; i < M; i++) {
        if (i & 1) t26_V2N[i] = tmp[i];
        else t26_V2P[i] = tmp[i];
    }
    for (r = 0; r < 8; r++) {
        t26_mul_T(t26_V2P, M);
        t26_mul_T(t26_V2N, M);
    }

    mem_free(s3);
    mem_free(r8);
    mem_free(tmp);
    t26_cap = C;
    t26_built = 1;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t i, k;

    if (!t26_built || n > t26_cap) {
        idx_t C = n;
        if (t26_built && t26_cap + t26_cap / 2 > C) C = t26_cap + t26_cap / 2;
        if (C < 100) C = 100;
        t26_build(C);
    }

    for (i = 0; i <= n; i++) pos += t26_A13[i] * t26_A13[n - i];

    for (k = 0; 2 * k + 1 <= n; k++) {
        seq_t t = t26_A14[n - 1 - 2 * k] * 52;
        pos += t * t26_V1N[k];
        neg += t * t26_V1P[k];
    }
    for (k = 0; 2 * k + 2 <= n; k++) {
        seq_t t = t26_R2[n - 2 - 2 * k] * 156;
        pos += t * t26_V2P[k];
        neg += t * t26_V2N[k];
    }
    return pos - neg;
}
