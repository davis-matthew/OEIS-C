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
 * Helpers:   t26_mul_T, t26_mul_theta, t26_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/t26_mul_T.h"
#include "../utils/t26_mul_theta.h"
#include "../utils/t26_zero.h"

static seq_t *t26_A13 = 0, *t26_A14 = 0, *t26_R2 = 0;
static seq_t *t26_V1P = 0, *t26_V1N = 0, *t26_V2P = 0, *t26_V2N = 0;
static idx_t t26_cap = 0;
static int t26_built = 0;

static void t26_build(idx_t C)
{
    idx_t L = C + 1, M = C / 2 + 1, i, d, j, r;
    seq_t *s3, *r8, *tmp;

    if (t26_built) {
        free(t26_A13); free(t26_A14); free(t26_R2);
        free(t26_V1P); free(t26_V1N); free(t26_V2P); free(t26_V2N);
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

    free(s3);
    free(r8);
    free(tmp);
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
