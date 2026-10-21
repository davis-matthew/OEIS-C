/*
 * A093275  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 75.
 * https://oeis.org/A093275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=0; for n>=2, a(n)=((10^(n-2)-1)*100+75)*10^(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    idx_t r = n - 2;
    seq_t p = pow10(r);                /* 10^(n-2) */
    seq_t prefix = (p - 1) * 100 + 75; /* number formed by (n-2) nines followed by 75 */
    return prefix * p;                 /* append (n-2) zeros */
}
