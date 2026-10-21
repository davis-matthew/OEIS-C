/*
 * A068624  a(1) = 9; a(n+1) = smallest triangular number > a(n) formed by
 *          adding at least one digit to a(n).
 * https://oeis.org/A068624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   a(n) = 2*10^(2n-4) - 11*10^(n-2) + 15 for n>=4; initial values:
 *            a(1)=9, a(2)=91, a(3)=1891
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)91;
    if (n == 3) return (seq_t)1891;

    seq_t t1 = pow10(2*n - 4);   /* 10^(2n-4) */
    seq_t t2 = pow10(n - 2);     /* 10^(n-2) */

    seq_t result = 2 * t1;       /* 2*10^(2n-4) */
    result -= 11 * t2;           /* -11*10^(n-2) */
    result += 15;                /* +15 */

    return result;
}
