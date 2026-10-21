/*
 * A054615  a(n) = Sum_{d|n} phi(d)*8^(n/d).
 * https://oeis.org/A054615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)*8^(n/d)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"      /* static seq_t phi(idx_t m); */
#include "../utils/pow_uint.h" /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d2 = n / d;

            /* term for divisor d */
            seq_t phi_d   = phi(d);
            seq_t pow_d2  = pow_uint(8, d2);
            sum += phi_d * pow_d2;

            if (d2 != d)
            {
                /* term for paired divisor d2 */
                seq_t phi_d2  = phi(d2);
                seq_t pow_d   = pow_uint(8, d);
                sum += phi_d2 * pow_d;
            }
        }
    }

    return sum;
}
