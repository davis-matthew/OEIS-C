/*
 * A002400  Coefficients for step-by-step integration.
 * https://oeis.org/A002400
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,2) * lcm(1,...,n+1) * Integral_{s=0..1}
 *            s*(s+1)*Product_{i=3..n} (s+i) ds (the (n+1)-point
 *            Adams-Bashforth weight of f(x_{-2}) over the denominator
 *            n!*lcm(1..n+1)); with P(s) = Sum_k c_k s^k: a(n) = binomial(n,2)
 *            * Sum_{k=0..n} c_k * lcm(1..n+1)/(k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A002400_MAX 40000

static seq_t a002400_c[A002400_MAX + 2];

seq_t a(idx_t n)
{
    idx_t i, k, deg, p, q, pp, top;
    seq_t L, sum;
    seq_t *c = a002400_c;

    if (n < 2) return (seq_t)0;
    if (n > A002400_MAX) return (seq_t)0;

    /* L = lcm(1..n+1) */
    top = n + 1;
    L = (seq_t)1;
    for (p = 2; p <= top; p++) {
        int isp = 1;
        for (q = 2; q * q <= p; q++)
            if (p % q == 0) { isp = 0; break; }
        if (!isp) continue;
        pp = p;
        while (pp <= top / p) pp *= p;
        L *= (seq_t)pp;
    }

    /* polynomial P(s) = prod_{i=0..n, i != 2} (s + i) */
    for (k = 0; k <= n + 1; k++) c[k] = (seq_t)0;
    c[0] = (seq_t)1;
    deg = 0;
    for (i = 0; i <= n; i++) {
        if (i == 2) continue;
        c[deg + 1] = c[deg];
        for (k = deg; k >= 1; k--)
            c[k] = c[k - 1] + c[k] * (seq_t)i;
        c[0] = c[0] * (seq_t)i;
        deg++;
    }

    sum = (seq_t)0;
    for (k = 0; k <= deg; k++)
        sum += c[k] * (L / (seq_t)(k + 1));

    return sum * (seq_t)(n * (n - 1) / 2);
}
