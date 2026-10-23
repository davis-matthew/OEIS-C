/*
 * A177845  a(n) = 6*a(n-1)-8*a(n-2)-3 for n > 2; a(0)=775, a(1)=8919,
 *          a(2)=34223.
 * https://oeis.org/A177845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=775; a(n)=2*4^(n+5)+91*2^(n+2)-1 (for n>0) (OEIS A177845)
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)775;

    /* compute 2*4^(n+5) + 91*2^(n+2) - 1 */
    seq_t term1 = (seq_t)2 * pow4(n + 5);
    seq_t term2 = (seq_t)91 * pow2(n + 2);
    return term1 + term2 - (seq_t)1;
}
