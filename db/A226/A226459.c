/*
 * A226459  a(n) = Sum_{d|n} phi(d^d), where phi(n) is the Euler totient
 *          function A000010(n).
 * https://oeis.org/A226459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d^d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, calc_phi, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* first divisor d1 = i */
            idx_t d1 = i;
            seq_t base1 = (seq_t)d1;
            seq_t exp1  = (seq_t)(d1 - 1);
            seq_t pow1  = big_pow(base1, exp1);
            seq_t phi1  = calc_phi(base1);
            sum += seq_mul(pow1, phi1);

            /* second divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t base2 = (seq_t)d2;
                seq_t exp2  = (seq_t)(d2 - 1);
                seq_t pow2  = big_pow(base2, exp2);
                seq_t phi2  = calc_phi(base2);
                sum += seq_mul(pow2, phi2);
            }
        }
    }
    return sum;
}
