/*
 * A087405  First differences of A087404: a(n) = A087404(n) - A087404(n-1),
 *          a(0) = A087404(0).
 * https://oeis.org/A087405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2, a(n) = 4*5^(n-1)+2*(-1)^n for n>0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t pow5 = (seq_t)1;
    /* compute 5^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i)
        pow5 *= (seq_t)5;

    seq_t term = (seq_t)4 * pow5;
    if (n & (idx_t)1)       /* n is odd: (-1)^n = -1 */
        term -= (seq_t)2;   /* subtract 2 */
    else
        term += (seq_t)2;   /* add 2 for even n */

    return term;
}
