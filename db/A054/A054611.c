/*
 * A054611  a(n) = Sum_{d|n} phi(d)*4^(n/d).
 * https://oeis.org/A054611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * 4^{n/d} (a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k = n / d;

            /* term for divisor d : phi(d) * 4^{k}  */
            seq_t term_d = phi(d) * pow2(2 * k);
            sum += term_d;

            if (d != k) {
                /* term for divisor k : phi(k) * 4^{d}  */
                seq_t term_k = phi(k) * pow2(2 * d);
                sum += term_k;
            }
        }
    }
    return sum;
}
