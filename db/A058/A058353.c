/*
 * A058353  n*A000084(n).
 * https://oeis.org/A058353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A000084(n), where A000084 has Euler transform
 *            Product_{k>=1} 1/(1-x^k)^b(k) = 1 + Sum_{k>=1} A000084(k)*x^k,
 *            b(1) = 1, b(k) = A000084(k)/2 for k >= 2, A000084(0) = 1,
 *            A000084(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-07)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

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

static seq_t A000084_a(idx_t n)
{
    seq_t x = 1;
    int w = 0;
    while (w < 300 && x != 0) {
        x = x * 2;
        w++;
    }
    if (n == 0)
        return 1;
    if (w < 300) {
        if (n > (idx_t)w || n > 128)
            return 0;
        return a84_small(n);
    }
    return a84_big(n);
}

seq_t a(idx_t n)
{
    return (seq_t)n * A000084_a(n);
}
