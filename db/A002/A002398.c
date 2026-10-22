/*
 * A002398  Coefficients for step-by-step integration.
 * https://oeis.org/A002398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1..n+1) * Integral_{x=0..1} Product_{k=1..n} (x+k) dx
 *            = Sum_{i=0..n} |Stirling1(n+1,i+1)| * lcm(1..n+1)/(i+1)
 *            (numerator of the Adams-Bashforth coefficient of f(x0), scaled
 *            by n!*lcm(1..n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   a002398_is_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a002398_is_prime_power.h"

#define A002398_MAXN 20000

static seq_t a002398_c[A002398_MAXN + 2];

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long m, k, i, p;
    seq_t L = 1;
    seq_t sum = 0;
    seq_t *c = a002398_c;

    if (N > A002398_MAXN)
        N = A002398_MAXN;

    for (m = 2; m <= N + 1; m++)
        if (a002398_is_prime_power(m, &p))
            L *= (idx_t)p;

    /* c[i] = coefficient of x^i in Product_{k=1..N} (x+k) */
    c[0] = 1;
    for (k = 1; k <= N; k++) {
        c[k] = c[k - 1];
        for (i = k - 1; i >= 1; i--)
            c[i] = c[i] * (idx_t)k + c[i - 1];
        c[0] = c[0] * (idx_t)k;
    }

    for (i = 0; i <= N; i++)
        sum += c[i] * (L / (idx_t)(i + 1));

    return sum;
}
