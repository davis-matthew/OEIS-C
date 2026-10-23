/*
 * A194560  G.f.: Sum_{n>=1} G_n(x)^n where G_n(x) = x + x*G_n(x)^n.
 * https://oeis.org/A194560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} C(n,d)/(n-d+1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 73, u128 n <= 139,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n=0, return 0 for safety */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* first divisor d */
            seq_t term = binomial(n, d);
            term /= (seq_t)(n - d + 1);
            sum += term;

            /* paired divisor n/d if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t term2 = binomial(n, d2);
                term2 /= (seq_t)(n - d2 + 1);
                sum += term2;
            }
        }
    }
    return sum;
}
