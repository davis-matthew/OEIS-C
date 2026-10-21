/*
 * A217670  G.f.: Sum_{n>=0} x^n/(1 + x^n)^n.
 * https://oeis.org/A217670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 when n = 0; for n > 0: a(n) = sum_{d|n} (-1)^(d-1) *
 *            binomial(d + n/d - 2, d - 1)
 * Range:     i8 n <= 23, i16 n <= 90, i32 n <= 314, i64 n <= 1146,
 *            i128 n <= 4319, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* using the provided gcd generator */
#include "../utils/gcd.h"

/* multiplicative binomial calculation avoiding overflow */
static seq_t binomial_multiplicative(idx_t nn, idx_t kk)
{
    if (kk > nn)
        return 0;
    if (kk > nn - kk)
        kk = nn - kk;

    /* numerator terms (nn-kk+1 … nn) and denominator terms (1 … kk) */
    seq_t num[128]; /* kk <= 128 for n <= 10000, safe on stack */
    seq_t den[128];
    for (idx_t i = 0; i < kk; ++i) {
        num[i] = (seq_t)(nn - kk + 1 + i);
        den[i] = (seq_t)(i + 1);
    }

    /* cancel common factors to keep numbers small */
    for (idx_t i = 0; i < kk; ++i) {
        if (den[i] == 1)
            continue;
        for (idx_t j = 0; j < kk && den[i] > 1; ++j) {
            seq_t g = gcd(num[j], den[i]);
            if (g > 1) {
                num[j] /= g;
                den[i] /= g;
            }
        }
    }

    /* multiply remaining numerator factors */
    seq_t res = 1;
    for (idx_t i = 0; i < kk; ++i)
        res *= num[i];
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* divisor d */
        idx_t nd = n / d;
        seq_t term = binomial_multiplicative(d + nd - 2, d - 1);
        if ((d & 1u) == 0)          /* d even → sign = -1 */
            term = -term;
        sum += term;

        /* paired divisor nd, if distinct */
        if (nd != d) {
            idx_t d2 = nd;
            idx_t nd2 = n / d2;   /* which is d */
            term = binomial_multiplicative(d2 + nd2 - 2, d2 - 1);
            if ((d2 & 1u) == 0)
                term = -term;
            sum += term;
        }
    }
    return sum;
}
