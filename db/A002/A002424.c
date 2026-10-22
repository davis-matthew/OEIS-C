/*
 * A002424  Expansion of (1-4*x)^(9/2).
 * https://oeis.org/A002424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = 2*(2*n-11)*a(n-1)/n, a(0) = 1 (i.e. a(n) =
 *            (-4)^n*binomial(9/2, n))
 * Range:     i8 n <= 2, i16 n <= 15, i32 n <= 26, i64 n <= 44, i128 n <= 78,
 *            big any n
 * Helpers:   a002424_sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002424_sgcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        long long m = 2 * (2 * (long long)k - 11);
        long long d = (long long)k;
        long long g = a002424_sgcd(m, d);
        m /= g;
        d /= g;
        /* d is coprime to m and divides result*m, so it divides result */
        result = result / (seq_t)d;
        result = result * (seq_t)m;
    }
    return result;
}
