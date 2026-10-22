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
 * Helpers:   binom14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom14.h"

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
