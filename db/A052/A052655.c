/*
 * A052655  a(2) = 6, otherwise a(n) = n*n!.
 * https://oeis.org/A052655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*n! for n >= 3 (a(2)=6, a(0)=0, a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)6;        /* special value */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;       /* compute n! */
    return n * fact;           /* n * n! */
}
