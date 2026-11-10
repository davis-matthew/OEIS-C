/*
 * A058499  G.f.: Product_{j>=1} (1 - x^j)^(-A000084(j)).
 * https://oeis.org/A058499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{j>=1} (1 - x^j)^(-A000084(j)); a(0) = 1. Fixed
 *            width: multiply in each factor 1/(1-x^k)^b(k) with b = A000084.
 *            Arbitrary precision: Euler transform recurrence n*a(n) =
 *            Sum_{k=1..n} c(k)*a(n-k), c(k) = Sum_{m|k} m*b(m)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-07)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

static seq_t a84_small(idx_t n)
{
    seq_t P[130];
    seq_t C[130];
    idx_t i, k, j, m;
    for (i = 0; i <= n; i++)
        P[i] = 0;
    P[0] = 1;
    for (k = 1; k <= n; k++) {
        seq_t b = (k == 1) ? (seq_t)1 : P[k];
        idx_t J = n / k;
        C[0] = 1;
        for (j = 1; j <= J; j++) {
            unsigned long long r = (unsigned long long)(C[j - 1] % (seq_t)j);
            unsigned long long g = gcd_u64(r, (unsigned long long)j);
            seq_t num = b + (seq_t)(j - 1);
            C[j] = (C[j - 1] / (seq_t)g) * (num / (seq_t)(j / g));
        }
        for (m = n; m >= k; m--) {
            seq_t add = 0;
            for (j = 1; j * k <= m; j++)
                add += C[j] * P[m - j * k];
            P[m] += add;
        }
    }
    return P[n];
}

#define A84_MAXN 30000

static seq_t a84_A[A84_MAXN + 1];
static seq_t a84_B[A84_MAXN + 1];
static seq_t a84_D[A84_MAXN + 1];
static idx_t a84_top = 0;

static seq_t a84_big(idx_t n)
{
    if (a84_top == 0) {
        a84_A[0] = 1;
        a84_A[1] = 1;
        a84_B[1] = 1;
        a84_D[1] = 1;
        a84_top = 1;
    }
    while (a84_top < n) {
        idx_t m = a84_top + 1;
        seq_t s = 0;
        seq_t t;
        idx_t q, k;
        for (q = 1; q <= m / 2; q++)
            if (m % q == 0)
                s += a84_B[q] * (seq_t)q;
        t = s;
        for (k = 1; k < m; k++)
            t += a84_D[k] * a84_A[m - k];
        a84_A[m] = t * 2 / (seq_t)m;
        a84_B[m] = a84_A[m] / 2;
        a84_D[m] = s + a84_B[m] * (seq_t)m;
        a84_top = m;
    }
    return a84_A[n];
}

static int a84_width(void)
{
    seq_t x = 1;
    int w = 0;
    while (w < 300 && x != 0) {
        x = x * 2;
        w++;
    }
    return w;
}

static seq_t A000084_a(idx_t n)
{
    int w = a84_width();
    if (n == 0)
        return 1;
    if (w < 300) {
        if (n > (idx_t)w || n > 128)
            return 0;
        return a84_small(n);
    }
    return a84_big(n);
}

static seq_t fixed_compute(idx_t n)
{
    seq_t *P = mem_new(seq_t, (size_t)(n + 1));
    seq_t *e = mem_new(seq_t, (size_t)(n + 1));
    idx_t m, k, j;
    seq_t res;
    for (m = 0; m <= n; m++)
        P[m] = 0;
    P[0] = 1;
    for (k = 1; k <= n; k++) {
        seq_t b = A000084_a(k);
        idx_t J = n / k;
        e[0] = 1;
        for (j = 1; j <= J; j++) {
            unsigned long long jj = (unsigned long long)j;
            unsigned long long g = gcd_u64((unsigned long long)(e[j - 1] % (seq_t)jj), jj);
            seq_t c = e[j - 1] / (seq_t)g;
            jj /= g;
            e[j] = c * ((b + (seq_t)(j - 1)) / (seq_t)jj);
        }
        for (m = n; m >= k; m--) {
            seq_t s = P[m];
            for (j = 1; j * k <= m; j++)
                s += e[j] * P[m - j * k];
            P[m] = s;
        }
    }
    res = P[n];
    mem_free(P);
    mem_free(e);
    return res;
}

static seq_t big_compute(idx_t n)
{
    seq_t *c = mem_new(seq_t, (size_t)(n + 1));
    seq_t *A = mem_new(seq_t, (size_t)(n + 1));
    idx_t m, i, k;
    seq_t res;
    for (m = 1; m <= n; m++) {
        seq_t t = (seq_t)m * A000084_a(m);
        for (i = m; i <= n; i += m)
            c[i] += t;
    }
    A[0] = 1;
    for (i = 1; i <= n; i++) {
        seq_t s = 0;
        for (k = 1; k <= i; k++)
            s += c[k] * A[i - k];
        A[i] = s / (seq_t)i;
    }
    res = A[n];
    mem_free(c);
    mem_free(A);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (a84_width() >= 300)
        return big_compute(n);
    if (n > 128)
        return 0;
    return fixed_compute(n);
}
