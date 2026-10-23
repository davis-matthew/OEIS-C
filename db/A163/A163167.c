/*
 * A163167  a(n) = sum_{d | phi(n)} mu( phi(d) ) * phi(n)/d, where phi =
 *          A000010.
 * https://oeis.org/A163167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{d | φ(n)} μ( φ(d) ) * φ(n)/d
 * Range:     u8 n <= 178, u16 n <= 9649, u32 n <= 9649, u64 n <= 9649,
 *            u128 n <= 9649, big any n
 * Helpers:   mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t phi_n = phi(n);
    idx_t phi_val = (idx_t)phi_n;

    int64_t sum = 0;
    for (idx_t d = 1; d * d <= phi_val; ++d)
    {
        if (phi_val % d == 0)
        {
            seq_t phi_d = phi(d);
            idx_t phi_d_val = (idx_t)phi_d;
            int mu = (int)mobius(phi_d_val);
            seq_t term = phi_n / (seq_t)d;
            sum += (int64_t)mu * (int64_t)term;

            idx_t other = phi_val / d;
            if (other != d)
            {
                seq_t phi_other = phi(other);
                idx_t phi_other_val = (idx_t)phi_other;
                int mu_other = (int)mobius(phi_other_val);
                seq_t term_other = phi_n / (seq_t)other;
                sum += (int64_t)mu_other * (int64_t)term_other;
            }
        }
    }
    return (seq_t)sum;
}
