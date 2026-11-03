/*
 * A007995  Poincaré series [or Poincare series] of Lie algebra associated
 *          with a certain braid group.
 * https://oeis.org/A007995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (5*x - 40*x^2 + 235*x^3 - 745*x^4 + 1586*x^5 - 3300*x^6 +
 *            7695*x^7 - 15480*x^8 + 22710*x^9 - 23430*x^10 + 16875*x^11 -
 *            8340*x^12 + 2705*x^13 - 520*x^14 + 45*x^15)/(1-x)^15, so a(n) =
 *            Sum_{k=1..15} c_k * binomial(n-k+14, 14)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom14(uint64_t m) {
    /* C(m,14) for m >= 14: cancel 14! from the 14 factors natively, then multiply in seq_t */
    uint64_t f[14];
    static const int primes[6] = {2, 3, 5, 7, 11, 13};
    static const int exps[6] = {11, 5, 2, 2, 1, 1};
    int i, j, e;
    seq_t r = 1;
    for (i = 0; i < 14; i++) f[i] = m - (uint64_t)i;
    for (j = 0; j < 6; j++) {
        uint64_t p = (uint64_t)primes[j];
        e = exps[j];
        for (i = 0; i < 14 && e > 0; i++) {
            while (e > 0 && f[i] % p == 0) {
                f[i] /= p;
                e--;
            }
        }
    }
    for (i = 0; i < 14; i++) r *= (seq_t)f[i];
    return r;
}

seq_t a(idx_t n) {
    static const long coeffs[15] = {5, -40, 235, -745, 1586, -3300, 7695, -15480,
                                    22710, -23430, 16875, -8340, 2705, -520, 45};
    seq_t pos = 0, neg = 0;
    idx_t k;
    for (k = 1; k <= 15; k++) {
        if (n >= k) {
            seq_t b = binom14((uint64_t)(n - k + 14));
            long c = coeffs[k - 1];
            if (c > 0) pos += (seq_t)c * b;
            else neg += (seq_t)(-c) * b;
        }
    }
    return pos - neg;
}
