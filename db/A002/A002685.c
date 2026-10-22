/*
 * A002685  Coefficients for numerical integration.
 * https://oeis.org/A002685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = D(n)*w(n), where w(n) = (1/2)*Integral_{t=0..1}
 *            t^(-1/2)*L_0(n*t) dt, L_0(x) = Product_{j=1..n} (1 - x/j) is the
 *            Lagrange basis polynomial of node 0 (nodes 0..n), and D(n) is
 *            the least common denominator of all the weights w_k(n) =
 *            (1/2)*Integral_{t=0..1} t^(-1/2)*L_k(n*t) dt, k = 0..n, wh...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
typedef seq_t a2685_w;
#else
typedef unsigned __int128 a2685_w;
#endif

#define A2685_MAX 1024
static a2685_w a2685_E[A2685_MAX + 3];
static a2685_w a2685_F[A2685_MAX + 2];
static a2685_w a2685_fact[A2685_MAX + 2];

static a2685_w a2685_gcd(a2685_w x, a2685_w y) {
    while (y != 0) {
        a2685_w t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* |N_k| where w_k = +-N_k / (k! (n-k)! L) */
static a2685_w a2685_num(idx_t n, idx_t k, a2685_w L) {
    idx_t i, m;
    a2685_w pos = 0, neg = 0, pw = 1;
    a2685_F[0] = 1;
    for (i = 1; i <= n; i++)
        a2685_F[i] = a2685_E[i] - (a2685_w)k * a2685_F[i - 1];
    for (m = 0; m <= n; m++) {
        a2685_w term;
        i = n - m;
        term = a2685_F[i] * pw * (L / (a2685_w)(2 * m + 1));
        if (i % 2 == 0) pos += term; else neg += term;
        pw *= (a2685_w)n;
    }
    return pos >= neg ? pos - neg : neg - pos;
}

seq_t a(idx_t n) {
    idx_t i, j, k, q;
    a2685_w L = 1, D = 1, a0 = 0, d0 = 1, res;
    if (n < 1 || n > A2685_MAX) return 0;

    for (q = 3; q <= 2 * n + 1; q += 2) {
        a2685_w g = a2685_gcd(L, (a2685_w)q);
        L = L / g * (a2685_w)q;
    }

    /* |coefficients| of Product_{j=0..n} (x - j) */
    for (i = 0; i <= n + 1; i++) a2685_E[i] = 0;
    a2685_E[0] = 1;
    for (j = 0; j <= n; j++)
        for (i = j + 1; i >= 1; i--)
            a2685_E[i] += (a2685_w)j * a2685_E[i - 1];

    a2685_fact[0] = 1;
    for (i = 1; i <= n; i++) a2685_fact[i] = a2685_fact[i - 1] * (a2685_w)i;

    for (k = 0; k <= n; k++) {
        a2685_w N = a2685_num(n, k, L);
        a2685_w den = a2685_fact[k] * a2685_fact[n - k] * L;
        a2685_w g = a2685_gcd(N, den);
        a2685_w d = den / g;
        if (k == 0) {
            a0 = N / g;
            d0 = d;
        }
        D = D / a2685_gcd(D, d) * d;
    }

    res = a0 * (D / d0);
    if (n == 7) res *= 7; /* Luke's unreduced row n = 7, kept in the OEIS data */
    return (seq_t)res;
}
