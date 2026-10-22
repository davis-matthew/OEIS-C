/*
 * A006917  Theta series of laminated lattice LAMBDA_13^{max}.
 * https://oeis.org/A006917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Lambda_13^max = {x in Z^16 : x mod 2 in RM(1,4), Sum x == 0 (mod
 *            4), x_0 = x_a = 0, x_b + x_c = 0} with {0,a,b,c} a 2-flat
 *            (scaled by 1/sqrt 2). Hence, with theta(t) = Sum_{m in Z}
 *            t^(m^2), psi(t) = Sum_{k>=0} t^(k(k+1)/2), A(t) = Sum_{j even}
 *            r_12(j) t^j (r_12 = coefficients of theta^12): G.f.:...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1867,
 *            u128 n <= 8077, big any n
 * Helpers:   alloc_zero, mul_theta2, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mul_theta2.h"

static seq_t *T4 = 0, *T6 = 0, *R12 = 0, *G6 = 0, *G8 = 0, *H = 0, *J = 0;
static idx_t slen = 0;

/* v *= theta(t) */
static void mul_theta(seq_t *v, idx_t len)
{
    idx_t i, m;
    for (i = len; i-- > 0;)
        for (m = 1; m * m <= i; m++)
            v[i] += v[i - m * m] + v[i - m * m];
}

/* v *= psi(t^(2s)), exponents s*k*(k+1) */
static void mul_psi(seq_t *v, idx_t len, idx_t s)
{
    idx_t i, k;
    for (i = len; i-- > 0;)
        for (k = 1; s * k * (k + 1) <= i; k++)
            v[i] += v[i - s * k * (k + 1)];
}

static seq_t *copy_arr(const seq_t *src, idx_t len)
{
    seq_t *d = alloc_zero(len);
    idx_t i;
    for (i = 0; i < len; i++) d[i] = src[i];
    return d;
}

static void build(idx_t len)
{
    seq_t *w, *g;
    int r;
    if (slen) {
        release(T4, slen); release(T6, slen); release(R12, slen);
        release(G6, slen); release(G8, slen); release(H, slen); release(J, slen);
    }
    w = alloc_zero(len);
    w[0] = 1;
    for (r = 0; r < 4; r++) mul_theta(w, len);
    T4 = copy_arr(w, len);
    H = copy_arr(w, len);
    mul_theta2(H, len);
    for (r = 0; r < 2; r++) mul_theta(w, len);
    T6 = copy_arr(w, len);
    J = copy_arr(w, len);
    mul_psi(J, len, 2);
    for (r = 0; r < 6; r++) mul_theta(w, len);
    R12 = w;

    g = alloc_zero(len);
    g[0] = 1;
    for (r = 0; r < 6; r++) mul_psi(g, len, 1);
    G6 = copy_arr(g, len);
    for (r = 0; r < 2; r++) mul_psi(g, len, 1);
    G8 = g;
    slen = len;
}

seq_t a(idx_t n)
{
    seq_t t1 = 0, t2 = 0, t3 = 0;
    idx_t m, k;
    if (n + 1 > slen) {
        idx_t newlen = n + 1;
        if (slen && newlen < slen + slen / 2) newlen = slen + slen / 2;
        build(newlen);
    }
    if (n % 2 == 0) {
        for (m = 0; 2 * m * m <= n; m++) {
            t1 += R12[n - 2 * m * m];
            if (m > 0) t1 += R12[n - 2 * m * m];
        }
    }
    if (n >= 2) {
        idx_t M = n - 2;
        for (k = 0; k <= M; k++) {
            t2 += H[k] * G8[M - k];
            t3 += J[k] * G6[M - k];
        }
    }
    return t1 + t2 * 384 + t3 * 256;
}
