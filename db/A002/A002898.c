/*
 * A002898  Number of n-step closed paths on hexagonal lattice.
 * https://oeis.org/A002898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, nice
 * Formula:   a(n) = Sum_{k} (3k)!/(k!)^3 * binomial(k, n-2k) * 2^(n-2k) (from
 *            O.g.f. Sum_{k>=0} (3k)!/k!^3 * x^(2k)*(1+2x)^k); every term is
 *            nonnegative and at most a(n), so exact multiply-divide steps
 *            never overflow
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 193 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* returns x * prod(num) / prod(den), known to be an integer */
static seq_t g2898_mulfrac(seq_t x, uint64_t *num, int nn, uint64_t *den, int nd) {
    int i, j;
    for (i = 0; i < nn; i++)
        for (j = 0; j < nd; j++) {
            uint64_t g = gcd_u64(num[i], den[j]);
            if (g > 1) { num[i] /= g; den[j] /= g; }
        }
    for (j = 0; j < nd; j++)
        if (den[j] > 1) x /= (seq_t)den[j];
    for (i = 0; i < nn; i++)
        if (num[i] != 1) x *= (seq_t)num[i];
    return x;
}

seq_t a(idx_t n) {
    idx_t k0 = (n + 2) / 3, kmax = n / 2, i, k;
    seq_t c, b, t, sum;
    uint64_t num[5], den[5];
    if (k0 > kmax) return (seq_t)0;
    /* c = (3k0)!/(k0!)^3 */
    c = (seq_t)1;
    for (i = 0; i < k0; i++) {
        num[0] = 3; num[1] = 3 * i + 1; num[2] = 3 * i + 2;
        den[0] = i + 1; den[1] = i + 1;
        c = g2898_mulfrac(c, num, 3, den, 2);
    }
    /* b = binomial(k0, j0) */
    {
        idx_t j0 = n - 2 * k0;
        b = (seq_t)1;
        for (i = 0; i < j0; i++) {
            num[0] = k0 - i; den[0] = i + 1;
            b = g2898_mulfrac(b, num, 1, den, 1);
        }
        t = c * b;
        for (i = 0; i < j0; i++) t *= 2;
    }
    sum = t;
    for (k = k0; k < kmax; k++) {
        idx_t j = n - 2 * k;
        num[0] = 3; num[1] = 3 * k + 1; num[2] = 3 * k + 2;
        num[3] = j; num[4] = j - 1;
        den[0] = k + 1; den[1] = 4; den[2] = k + 3 - j;
        den[3] = k + 2 - j; den[4] = k + 1 - j;
        t = g2898_mulfrac(t, num, 5, den, 5);
        sum += t;
    }
    return sum;
}
