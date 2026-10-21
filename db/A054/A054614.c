/*
 * A054614  a(n) = Sum_{d|n} phi(d)*7^(n/d).
 * https://oeis.org/A054614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)*7^(n/d)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t phi1 = calc_phi((seq_t)d1);
            seq_t pow1 = big_pow(7, (seq_t)(n / d1));
            sum += phi1 * pow1;

            if (d1 != d2) {
                seq_t phi2 = calc_phi((seq_t)d2);
                seq_t pow2 = big_pow(7, (seq_t)d1); /* exponent = n/d2 = d1 */
                sum += phi2 * pow2;
            }
        }
    }
    return sum;
}
