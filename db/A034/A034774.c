/*
 * A034774  Dirichlet convolution of d(n) (# of divisors) with Catalan
 *          numbers.
 * https://oeis.org/A034774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} d(d) * C_{n/d - 1} , where d(d) is
 *            divisor_count_u64(d) and C_k is catalan(k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   catalan, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n<1; guard */

    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) /* iterate over divisors up to sqrt(n) */
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* contribution from divisor d1 */
            seq_t tau_d1 = num_divisors(d1);
            seq_t cat_d2  = catalan(d2 - 1);  /* C_{n/d2 -1} = C_{d1-1} */
            sum += tau_d1 * cat_d2;

            if (d1 != d2)                   /* second divisor d2 when d1!=d2 */
            {
                seq_t tau_d2 = num_divisors(d2);
                seq_t cat_d1 = catalan(d1 - 1);  /* C_{d2-1} */
                sum += tau_d2 * cat_d1;
            }
        }
    }

    return sum;
}
