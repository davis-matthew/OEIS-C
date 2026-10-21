/*
 * A060725  E.g.f.: exp(-(x^5/5))/(1-x).
 * https://oeis.org/A060725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{i=0}^{⌊n/5⌋} (-1)^i / (i! * 5^i)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t maxI = n / 5U;               /* maximum i in the sum */
    seq_t fact = 1;                    /* n! */

    /* Compute n! */
    for (idx_t k = 2; k <= n; ++k)
        fact *= (seq_t)k;

    seq_t term = fact;                 /* term for i = 0 : n! / (0! * 5^0) */
    seq_t sum  = 0;

    for (idx_t i = 0; i <= maxI; ++i) {
        if ((i & 1U) == 0)             /* add for even i */
            sum += term;
        else                           /* subtract for odd i */
            sum -= term;

        if (i != maxI) {               /* prepare term_{i+1} */
            idx_t denom = (i + 1U) * 5U;
            term /= (seq_t)denom;      /* exact division */
        }
    }

    return sum;
}
