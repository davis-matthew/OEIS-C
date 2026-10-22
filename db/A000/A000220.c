/*
 * A000220  Number of asymmetric trees with n nodes (also called identity
 *          trees).
 * https://oeis.org/A000220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   G.f. A(x) - A(x)^2/2 - A(x^2)/2, where A(x) = Sum b(n) x^n is
 *            the g.f. of A004111: b(1) = 1, b(n+1) = (1/n) * Sum_{k=1..n}
 *            c(k) b(n-k+1), c(k) = Sum_{d|k} (-1)^(k/d+1) d b(d). Hence a(n)
 *            = b(n) - Sum_{i=1..floor((n-1)/2)} b(i) b(n-i) - [n even]
 *            (b(n/2)^2 + b(n/2))/2. (Fixed-width builds compute b...
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 60, u128 n <= 110,
 *            big any n
 * Helpers:   a220_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a220_width.h"

#define A220_MAXN 10050
#define A220_FMAX 400

static seq_t a220_B[A220_MAXN + 2];
static seq_t a220_C[A220_MAXN + 2];
static seq_t a220_F[A220_FMAX + 2];
static seq_t a220_bin[A220_FMAX + 2];
static idx_t a220_have = 0;
static int a220_bits = -1;
static idx_t a220_fixN = 0;

/* C(m, j) computed without division of the (possibly wrapped) product. m exact. */
static seq_t a220_binom(seq_t m, idx_t j)
{
    seq_t t[A220_FMAX + 2];
    seq_t r;
    idx_t i, q;
    if (j == 0) return 1;
    if (m < (seq_t)j) return 0;
    for (i = 1; i <= j; i++) t[i] = m - (seq_t)(i - 1);
    for (q = 2; q <= j; q++) {
        idx_t rr = q, p;
        for (p = 2; rr > 1; p++) {
            while (rr % p == 0) {
                for (i = 1; i <= j; i++) {
                    if (t[i] % p == 0) {
                        t[i] /= p;
                        break;
                    }
                }
                rr /= p;
            }
        }
    }
    r = 1;
    for (i = 1; i <= j; i++) r *= t[i];
    return r;
}

static void a220_fixed_init(void)
{
    idx_t N = (idx_t)a220_bits + (idx_t)a220_bits / 2;
    idx_t k, i, j;
    if (N > A220_FMAX) N = A220_FMAX;
    a220_fixN = N;
    for (i = 0; i <= N; i++) a220_F[i] = 0;
    a220_F[0] = 1;
    for (k = 1; k <= N; k++) {
        a220_B[k] = a220_F[k - 1];
        if (k == N) break;
        for (j = 1; k * j <= N; j++) a220_bin[j] = a220_binom(a220_B[k], j);
        for (i = N; i >= k; i--) {
            seq_t add = 0;
            for (j = 1; k * j <= i; j++) add += a220_bin[j] * a220_F[i - k * j];
            a220_F[i] += add;
        }
    }
}

static void a220_extend(idx_t n)
{
    if (a220_have == 0) {
        a220_B[1] = 1;
        a220_C[1] = 1;
        a220_have = 1;
    }
    while (a220_have < n) {
        idx_t m = a220_have, k, d, t = m + 1;
        seq_t S = 0, pos, neg;
        for (k = 1; k <= m; k++) S += a220_C[k] * a220_B[m - k + 1];
        a220_B[t] = S / (seq_t)m;
        pos = (seq_t)t * a220_B[t];
        neg = 0;
        for (d = 1; d * d <= t; d++) {
            if (t % d == 0) {
                idx_t e = t / d;
                if (d < t) {
                    if (e & 1) pos += (seq_t)d * a220_B[d];
                    else neg += (seq_t)d * a220_B[d];
                }
                if (e != d && e < t) {
                    if (d & 1) pos += (seq_t)e * a220_B[e];
                    else neg += (seq_t)e * a220_B[e];
                }
            }
        }
        a220_C[t] = pos - neg;
        a220_have = t;
    }
}

seq_t a(idx_t n)
{
    seq_t T = 0;
    idx_t i;
    if (n < 1) return 0;
    if (a220_bits < 0) {
        a220_bits = a220_width();
        if (a220_bits < 300) a220_fixed_init();
    }
    if (a220_bits < 300) {
        if (n > a220_fixN) return 0;
    } else {
        if (n > A220_MAXN) return 0;
        a220_extend(n);
    }
    for (i = 1; 2 * i < n; i++) T += a220_B[i] * a220_B[n - i];
    if (n % 2 == 0) {
        seq_t h = a220_B[n / 2];
        T += (h * h + h) / 2;
    }
    return a220_B[n] - T;
}
