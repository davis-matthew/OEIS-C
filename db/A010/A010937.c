/*
 * A010937  Binomial coefficient C(21,n).
 * https://oeis.org/A010937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(21,n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Return 0 for n > 21 (binomial coefficient is zero outside the row) */
    if (n > 21u)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    /* Compute C(21,n) iteratively, reducing at each step to avoid overflow */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mul = (seq_t)(21u - i + 1u);  /* numerator factor */
        seq_t div = (seq_t)i;                /* denominator factor */

        /* Reduce result and divisor */
        seq_t g = gcd(result, div);
        result /= g;
        div /= g;

        /* Reduce numerator and divisor */
        g = gcd(mul, div);
        mul /= g;
        div /= g;   /* div should now be 1 */

        result *= mul;
        if (div != (seq_t)1)  /* just in case div != 1 */
            result /= div;
    }
    return result;
}
