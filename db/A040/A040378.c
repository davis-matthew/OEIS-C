/*
 * A040378  Continued fraction for sqrt(398).
 * https://oeis.org/A040378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=19; for n≥1: a(n)=1 if n odd; otherwise a(n)=18 if v2(n)=1,
 *            else a(n)=38
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;

    /* odd numbers give 1 */
    if (n & 1)
        return (seq_t)1;

    /* count exponent of 2 in n */
    idx_t exp2 = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++exp2;
    }

    if (exp2 == 1)
        return (seq_t)18;
    else
        return (seq_t)38;
}
