/*
 * A126901  Coordination sequence for 8-dimensional cyclotomic lattice
 *          Z[zeta_20].
 * https://oeis.org/A126901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^8 Q[j] * binomial(n-j+7,7) , where Q = [1, 12,
 *            68, 204, 330, 204, 68, 12, 1]
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the generating function
 * (x^4+6x^3+16x^2+6x+1)^2 / (1-x)^8. */

#include "../utils/gcd.h"

static seq_t binom_n_plus_7(idx_t k)
{
    seq_t res = 1;
    for (idx_t d = 1; d <= 7; ++d) {
        seq_t numerator = (seq_t)k + (seq_t)d;
        seq_t denom     = (seq_t)d;

        seq_t g = gcd(res, denom);
        if (g > 1) {
            res   /= g;
            denom /= g;
        }

        g = gcd(numerator, denom);
        if (g > 1) {
            numerator /= g;
            denom     /= g;
        }

        res = res * numerator;
        if (denom > 1)
            res = res / denom;
    }
    return res;
}

seq_t a(idx_t n)
{
    static const int Q[9] = {1, 12, 68, 204, 330, 204, 68, 12, 1};
    idx_t maxj = (n < 8) ? n : 8;
    seq_t result = 0;

    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t k    = n - j;
        seq_t coef = binom_n_plus_7(k);
        result += (seq_t)Q[j] * coef;
    }
    return result;
}
