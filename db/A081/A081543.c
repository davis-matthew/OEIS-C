/*
 * A081543  G.f.: Sum_{k >= 1} x^k/(1-x^k)^(k+1).
 * https://oeis.org/A081543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(d-1 + n/d, d)
 * Range:     u8 n <= 19, u16 n <= 79, u32 n <= 298, u64 n <= 1119,
 *            u128 n <= 4319, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1; a(0) = 0 by convention. */
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n / i) {
        if (n % i == 0) {
            idx_t d = i;
            idx_t e = n / i;
            sum += binom(d - 1 + e, d);
            if (d != e) {
                sum += binom(e - 1 + d, e);
            }
        }
        ++i;
    }
    return sum;
}
