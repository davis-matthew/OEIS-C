/*
 * A054585  Sum_{d=1..n} phi(d)*mu(d).
 * https://oeis.org/A054585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d=1..n} phi(d) * mu(d)
 * Range:     i8 n <= 42, i16 n <= 2388, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   calc_phi, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t mu = mobius(d);
        if (mu != (seq_t)0) {
            seq_t phi_d = calc_phi((seq_t)d);
            sum += phi_d * mu;
        }
    }
    return sum;
}
