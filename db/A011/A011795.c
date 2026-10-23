/*
 * A011795  a(n) = floor(C(n,4)/5).
 * https://oeis.org/A011795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n,4)/5) (offset 0)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 261, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* convert to the sequence type to avoid truncation */
    seq_t nn = (seq_t)n;
    seq_t v = nn * (nn - 1);           /* n*(n-1)          */
    v *= (nn - 2);                     /* * (n-2)          */
    v *= (nn - 3);                     /* * (n-3)          */
    v /= 24;                           /* divide by 4!      */
    return v / 5;                      /* floor(binomial/5) */
}
