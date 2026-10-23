/*
 * A173191  a(n) = binomial(n+6,6)*9^n.
 * https://oeis.org/A173191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6) * 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute binomial(n+6,6) using a multiplicative recurrence to avoid
       large factorials or intermediate overflows. */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        /* Multiply by (n + 7 - i), then divide by i.  The division is
           exact at each step because the binomial coefficient is integral. */
        binom = binom * (seq_t)(n + 7 - i) / i;
    }

    /* Compute 9^n using the supplied arbitrary‑precision power routine. */
    seq_t pow9 = pow_uint(9, n);

    /* Final result: binomial * 9^n */
    return binom * pow9;
}
