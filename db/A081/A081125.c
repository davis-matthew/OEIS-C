/*
 * A081125  a(n) = n! / floor(n/2)!.
 * https://oeis.org/A081125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{i=floor(n/2)+1}^{n} i = n! / floor(n/2)!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t d = n/2;                 /* floor(n/2)          */
    seq_t result = (seq_t)1;       /* accumulate product  */

    for (idx_t i = d + 1; i <= n; ++i)
        result *= (seq_t)i;        /* multiply safely     */

    return result;
}
