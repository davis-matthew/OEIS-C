/*
 * A230263  Number of nonnegative integer solutions to the equation x^2 -
 *          4*y^2 = n.
 * https://oeis.org/A230263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to x^2 - 4*y^2 =
 *            n computed by testing divisors f of n where (n - f^2) is
 *            divisible by 4*f
 * Range:     u8 n <= 475, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {            /* iterate f up to sqrt(n)   */
        if (n % i != 0) continue;             /* f must divide n            */
        seq_t f = (seq_t)i;
        seq_t numerator = (seq_t)n - f * f;   /* n - f^2                    */
        seq_t denom = 4 * f;                  /* 4*f                        */
        if (numerator % denom != 0) continue;   /* y must be integer          */
        seq_t y = numerator / denom;           /* compute y                  */
        /* y is nonnegative by construction   */
        /* compute x to ensure integrality (unnecessary for count) */
        (void)((seq_t)2 * y + f); /* cast to seq_t, value unused */
        ++count;
    }
    return count;
}
