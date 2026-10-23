/*
 * A106367  Number of necklaces with n beads of 5 colors, no 2 adjacent beads
 *          the same color.
 * https://oeis.org/A106367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(n/d) * (4*(-1)^d + 4^d) with a(1) =
 *            5, where phi is Euler's totient function
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   phi_uint, pow4_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple divisor loop and an inline phi calculation
   that does not rely on external utilities. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi_uint.h"
#include "../utils/pow4_uint.h"

/* --- Main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)5;

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            seq_t phi1   = phi_uint(n / d);
            seq_t pow4d  = pow4_uint(d);
            seq_t term   = pow4d;
            if (d & 1) term -= (seq_t)4; else term += (seq_t)4;
            term *= phi1;
            sum += term;

            /* paired divisor d2 = n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t phi2   = phi_uint(d);
                seq_t pow4d2 = pow4_uint(d2);
                seq_t term2  = pow4d2;
                if (d2 & 1) term2 -= (seq_t)4; else term2 += (seq_t)4;
                term2 *= phi2;
                sum += term2;
            }
        }
    }
    return sum / n;
}
