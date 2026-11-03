/*
 * A054615  a(n) = Sum_{d|n} phi(d)*8^(n/d).
 * https://oeis.org/A054615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)*8^(n/d)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 938 (2026-11-01)
 * Helpers:   ipow_u64_idx, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/totient.h"

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
            seq_t phi_d   = totient(d);
            seq_t pow_d2  = ipow_u64_idx(8, d2);
            sum += phi_d * pow_d2;

            if (d2 != d)
            {
                /* term for paired divisor d2 */
                seq_t phi_d2  = totient(d2);
                seq_t pow_d   = ipow_u64_idx(8, d);
                sum += phi_d2 * pow_d;
            }
        }
    }

    return sum;
}
